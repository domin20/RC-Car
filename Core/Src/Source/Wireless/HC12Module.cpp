#include "Wireless/HC12Module.h"

#include <main.h>
#include <string.h>

void HC12Module::init(UART_HandleTypeDef* huart, Function<uint64_t()> timeBaseUs) {
  this->huart = huart;
  this->timeBaseUs = timeBaseUs;

  HAL_UART_Receive_IT(huart, (uint8_t*)&receiverContext.rxByte, 1);
}

void HC12Module::sendFrame(const WirelessFrame& frame) {
  static uint8_t frameBuffer[MAX_FRAME_SIZE];
  uint8_t frameIdx = 0;
  uint16_t crc16 = 0;

  frameBuffer[frameIdx++] = frame.master;
  frameBuffer[frameIdx++] = frame.command;
  frameBuffer[frameIdx++] = frame.dataSize;
  memcpy(&frameBuffer[frameIdx], frame.data, frame.dataSize);
  frameIdx += frame.dataSize;

  crc16 = calculateCRC16(frameBuffer, frame.dataSize + 2);
  memcpy(&frameBuffer[frameIdx], &crc16, sizeof(crc16));
  frameIdx += sizeof(crc16);

  this->isTxCplt = false;
  HAL_UART_Transmit_IT(this->huart, frameBuffer, frameIdx);
}

const WirelessFrame& HC12Module::getReceivedFrame() {
  this->receiverContext.isRxCompleted = false;
  return this->receivedFrame;
}

bool HC12Module::isFrameAvailable() { return this->receiverContext.isRxCompleted; }

void HC12Module::update() {
  if (!this->timeBaseUs) {
    return;
  }
  if (this->isFrameReceiving) {
    if (this->timeBaseUs() - this->timeStampLastByte > _3_5_CHAR_SEND_TIME_US) {
      this->processFrame();
    }
  }
}

void HC12Module::setTxCpltFlag() { this->isTxCplt = true; }

void HC12Module::loadDataToFrameStruct() {
  uint16_t calculatedCRC16 = this->calculateCRC16((uint8_t*)this->receiverContext.rxFrame,
                                                  this->receiverContext.rxFrameSize - CRC_SIZE);
  uint16_t receivedCrc16 = 0;
  memcpy(&receivedCrc16,
         (uint8_t*)&this->receiverContext.rxFrame[this->receiverContext.rxFrameSize - CRC_SIZE],
         CRC_SIZE);

  if (calculatedCRC16 == receivedCrc16) {
    this->receiverContext.isRxCompleted = true;
    uint8_t frameIdx = 0;

    this->receivedFrame.master = this->receiverContext.rxFrame[frameIdx++];
    this->receivedFrame.command = this->receiverContext.rxFrame[frameIdx++];
    this->receivedFrame.dataSize = this->receiverContext.rxFrame[frameIdx++];

    memcpy(this->receivedFrame.data, (uint8_t*)&this->receiverContext.rxFrame[frameIdx],
           this->receivedFrame.dataSize);
    this->receivedFrame.crc16 = receivedCrc16;
  }
}

void HC12Module::onReceivedData(void) {
  if (!this->timeBaseUs) {
    return;
  }
  this->timeStampLastByte = this->timeBaseUs();
  this->isFrameReceiving = true;

  this->receiverContext.rxFrame[this->receiverContext.rxFrameSize] = this->receiverContext.rxByte;
  this->receiverContext.rxFrameSize++;

  HAL_UART_Receive_IT(this->huart, (uint8_t*)&receiverContext.rxByte, 1);
}

void HC12Module::processFrame() {
  if (this->receiverContext.rxFrameSize >= MIN_FRAME_SIZE) {
    this->loadDataToFrameStruct();
  }
  this->receiverContext.rxFrameSize = 0;
  this->isFrameReceiving = false;
}

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
