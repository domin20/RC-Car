#include "Wireless/SecurityLayers/RotatingKey.h"

bool RotatingKey::isValid(uint64_t key) {}

uint64_t RotatingKey::generateNewKey() {
  uint64_t k = 0;
  while (k == 0) {
    uint64_t m32 = this->keyGen(this->generator);
    uint64_t l32 = this->keyGen(this->generator);

    k = (m32 << 32) | l32;
  }
  return k;
}

void RotatingKey::updateKeys() {
  this->prevKey = this->newKey;
  this->newKey = this->tempNewKey;
}

void RotatingKey::encrypt(uint8_t* data, size_t size) {
  this->tempNewKey = this->generateNewKey();
  memcpy(&data[size - sizeof(this->tempNewKey)], &this->tempNewKey, sizeof(this->tempNewKey));
  this->xorWithKey(data, size, this->newKey);
}

void RotatingKey::encryptUsingSameKey(uint8_t* data, size_t size) {
  memcpy(&data[size - sizeof(this->newKey)], &this->newKey, sizeof(this->newKey));
  this->xorWithKey(data, size, this->prevKey);
}

bool RotatingKey::decrypt(uint8_t* data, size_t size) {
  if (!data) {
    return false;
  }
  this->xorWithKey(data, size, this->newKey);
  memcpy(&this->tempNewKey, (void*)(data + (size - sizeof(this->tempNewKey))),
         sizeof(this->tempNewKey));
  return true;
}

bool RotatingKey::decryptUsingPreviousKey(uint8_t* data, size_t size) {
  if (!data) {
    return false;
  }
  this->xorWithKey(data, size, this->prevKey);
  return true;
}

void RotatingKey::xorWithKey(uint8_t* data, size_t size, uint64_t key) {
  uint8_t keyBytes[8];
  memcpy(keyBytes, &key, sizeof(key));

  // Przetwarzanie danych przez XOR z kluczem
  for (size_t i = 0; i < size; ++i) {
    data[i] ^= keyBytes[i % 8];
  }
}
