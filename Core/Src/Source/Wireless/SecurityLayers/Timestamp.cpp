#include "Wireless/SecurityLayers/Timestamp.h"

#include <time.h>

#include "AppHelper/Env/AppEnvironment.h"

bool Timestamp::isValid(uint64_t key) {
  time_t currentTime = AppEnvironment::getEnvironmentContext()->getUnixTimeFromRTC();
  if (key >= (uint64_t)currentTime - 2 && key <= (uint64_t)currentTime + 2) {
    return true;
  } else {
    return false;
  }
}

uint64_t Timestamp::generateNewKey() {
  time_t currentTime = AppEnvironment::getEnvironmentContext()->getUnixTimeFromRTC();
  return currentTime;
}

void Timestamp::updateKeys() {}

void Timestamp::encrypt(uint8_t* data, size_t size) {
  auto timestampUnix = this->generateNewKey();

  uint64_t encryptedTs = this->processTimestampUsingXor(timestampUnix, data, size);
  memcpy(&data[size - sizeof(encryptedTs)], &encryptedTs, sizeof(encryptedTs));
}

void Timestamp::encryptUsingSameKey(uint8_t* data, size_t size) {}

bool Timestamp::decrypt(uint8_t* data, size_t size) {
  if (!data) {
    return false;
  }
  uint64_t encryptedTs = 0;
  memcpy(&encryptedTs, &data[size - sizeof(encryptedTs)], sizeof(encryptedTs));

  uint64_t decryptedTs = this->processTimestampUsingXor(encryptedTs, data, size);
  uint64_t timestamp = AppEnvironment::getEnvironmentContext()->getUnixTimeFromRTC();
  if (decryptedTs >= timestamp - 1 && decryptedTs <= timestamp + 1) {
    return true;
  }
  return false;
}

bool Timestamp::decryptUsingPreviousKey(uint8_t* data, size_t size) { return true; }

uint64_t Timestamp::processTimestampUsingXor(uint64_t timestamp, uint8_t* data, uint8_t size) {
  // dataSize is third byte in WirelessFrame
  uint8_t dataSize = data[2];

  // master + command + dataSize + data[] + CRC16
  // 5 = frame without dataSize and key > frame size without key
  if (dataSize + 5 == size - sizeof(uint64_t)) {
    uint8_t keyBytes[8] = {0};
    uint8_t idx = 0;
    memcpy(&keyBytes[idx], &data[size - sizeof(uint64_t) - CRC_SIZE], CRC_SIZE);
    idx += CRC_SIZE;
    memcpy(&keyBytes[idx], &data[3], dataSize > 6 ? 6 : dataSize);

    uint8_t* tsPointer = reinterpret_cast<uint8_t*>(&timestamp);
    for (size_t i = 0; i < sizeof(timestamp); ++i) {
      tsPointer[i] ^= keyBytes[i % 8];
    }
    return timestamp;
  }
  return 0;
}
