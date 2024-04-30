#include "Wireless/HC12Module.h"

#include <main.h>

// extern UART_HandleTypeDef huart2;

extern volatile uint8_t radio_flag;
extern volatile uint8_t radio_buffer[20];
extern volatile uint8_t byte;

void HC12Module::init(Function<uint64_t()> timeBase) { this->timeBase = timeBase; }

void HC12Module::sendFrame(const WirelessFrame& frame) {}

const WirelessFrame& HC12Module::getReceivedFrame() {}

bool HC12Module::isFrameAvailable() {}

void HC12Module::update() {}

void HC12Module::performFrame() {}
void HC12Module::loadDataToFrame() {}

uint16_t HC12Module::calculateCRC16(uint8_t* data, uint8_t length) {
  uint16_t crc = 0xFFFF;

  for (int pos = 0; pos < length; pos++) {
    crc ^= (uint16_t)data[pos];  // XOR byte into least sig. byte of crc

    for (int i = 8; i != 0; i--) {  // Loop over each bit
      if ((crc & 0x0001) != 0) {    // If the LSB is set
        crc >>= 1;                  // Shift right and XOR 0xA001
        crc ^= 0xA001;
      } else {      // Else LSB is not set
        crc >>= 1;  // Just shift right
      }
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return crc;
}

void send_radio_frame(uint8_t* data_bytes, uint8_t size, uint8_t command) {
  // if(size > MAX_DATA_BYTES) return; // jesli ilosc bajtow danych sie nie zgadza zakoncz dzialanie
  // funkcji
  uint8_t frame[size + 5];   // zmienna przechowujaca dane ramki do wyslania
  uint16_t check_bytes = 0;  // bajty sprawdzajace (dwa ostatnie w ramce)
  frame[0] = 255;            // pierwszy bajt = 255
  frame[1] =
      command;      // adresy 4 starsze bity adres urzadzenia odbiorczego, 4 mlodsze bity - nadawca
  frame[2] = size;  // komendy
  // frame[3] = size;			// rozmiar danych
  for (uint8_t i = 3; i < size + 3; i++) {
    frame[i] = *data_bytes++;  // zaladuj dane do ramki
    if (frame[i] == 255) frame[i] = 254;
    check_bytes += frame[i];  // dodaj wartosci danych do zmiennej sprawdzajace
  }
  for (uint8_t i = 0; i < 3; i++) check_bytes += frame[i];

  if ((check_bytes & 0xFF) == 255)
    check_bytes <<= 1;  // jesli drugi bajt sprawdzajacy == 255 przesun bitowo o 1 w lewo
  frame[size + 3] = check_bytes >> 8;  //
  frame[size + 4] = check_bytes;       // zaladuj bajty sprawdzajace do ramki

  // HAL_UART_Transmit_IT(&huart2, frame, size + 5);	// wyslij cala ramke
}

void get_radio_frame(void) {
  static uint8_t cnt = 0;             // zmienna licznikowa
  static uint8_t size, frame[20];     // size - zmienna przechowujaca ilosc bajtow danych/frame -
                                      // zmienna przechowujaca ilosc bajtow w ramce
  uint16_t sum = 0, check_bytes = 0;  // bajty sprawdzajace (dwa ostatnie w ramce)
  if (byte == 255) {
    frame[0] = byte;
    cnt = 1;
    return;
  }                // jesli rowny 255 -> pierwszy bajt ramki
  if (cnt == 1) {  // numer polecenia
    frame[1] = byte;
    if (frame[1] > 0) {
      cnt = 2;
      return;
    } else {
      cnt = 0;
      return;
    }
  }
  if (cnt == 2) {
    frame[2] = byte;
    cnt = 3;
    return;
  }  // rozmiar danych

  if ((cnt >= 3) && (cnt < size + 5))  // odbieramy bajty danych + sume kontrolna..
  {
    frame[cnt] = byte;    // zapisujemy do miejsca w ramce
    cnt++;                // zwiekszamy licznik
    if (cnt == size + 5)  // jesli odebralismy juz wszystkie dane..
    {
      check_bytes = frame[cnt - 2];   //
      check_bytes <<= 8;              //
      check_bytes |= frame[cnt - 1];  //	odkodowywujemy check_bytes

      for (uint8_t i = 0; i < size + 3; i++) {
        sum += frame[i];  // sprawdzamy zgodnosc sumy wszystkich bajtow i 1-go bajtu z check_bytes
      }

      if ((sum & 0xFF) == 255)
        sum <<= 1;             // jesli drugi bajt sprawdzajacy = 255 przesun bitowo o 1 w lewo
      if (sum == check_bytes)  // jesli jest rowna..
      {
        cnt = 0;  // zerujemy licznik
        for (uint8_t i = 1; i < size + 3; i++) {
          radio_buffer[i - 1] = frame[i];  // zapisujemy dane do bufora
        }
        radio_flag |= 0x01;  // ustawiamy flage odebranych i prawidlowych danych
      } else {
        cnt = 0;
        radio_flag &= ~0x01;
      }  // jesli nie ma zgodnosci sumy wyzeruj licznik i flage
    }
  }
}
