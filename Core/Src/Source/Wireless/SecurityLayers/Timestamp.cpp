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

void Timestamp::encrypt(uint8_t* data, size_t size) {}

void Timestamp::encryptUsingSameKey(uint8_t* data, size_t size) {}

void Timestamp::decrypt(uint8_t* data, size_t size) {}

void Timestamp::decryptUsingPreviousKey(uint8_t* data, size_t size) {}
