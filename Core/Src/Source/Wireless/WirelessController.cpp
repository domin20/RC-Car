#include "Wireless/WirelessController.h"

#include <string.h>

#include "App.h"

void WirelessController::init(HC12Module* module) {
  this->radioModule = module;
  this->radioModule->setByteUsedAsReceivedAck(this->ackByte);
}

void WirelessController::send(uint8_t command, uint8_t dataSize, void* data) {
  this->txFrame.master = MASTER_NUMBER;
  this->txFrame.command = command;
  this->txFrame.dataSize = dataSize;
  memcpy(this->txFrame.data, data, dataSize);

  this->txFrame.crc16 =
      calculateCRC16(reinterpret_cast<uint8_t*>(&this->txFrame), txFrame.dataSize + 3);
  uint8_t frameSize = this->txFrame.ToBuffer(this->txFrameBuffer, 255);

  assert(this->securityLayerPair != nullptr);

  if (this->securityLayerPair->second) {
    if (this->securityLayerPair->first.isAckReceived) {
      this->sendEncryptedFrame(frameSize);
    } else if (this->securityLayerPair->first.isAckReceived == false &&
               this->isAckWaitingTimePassed()) {
      this->sendEncryptedFrameUsingSameKey(frameSize);
    } else if (this->securityLayerPair->first.isAckReceived == false &&
               !this->isAckWaitingTimePassed()) {
      return;
    }
  }
}

void WirelessController::sendEncryptedFrame(uint8_t frameSize) {
  assert(this->securityLayerPair != nullptr);
  this->securityLayerPair->second->encrypt(this->txFrameBuffer, frameSize);
  auto result = this->radioModule->sendFrame(this->txFrameBuffer, frameSize);
  if (this->securityLayerPair->second && result) {
    this->securityLayerPair->second->updateKeys();
    this->securityLayerPair->first.lastTxFrameTimestamp =
        AppEnvironment::getEnvironmentContext()->timeBaseMs();
    this->securityLayerPair->first.isAckReceived = false;
  }
}

void WirelessController::sendEncryptedFrameUsingSameKey(uint8_t frameSize) {
  assert(this->securityLayerPair != nullptr);
  this->securityLayerPair->second->encryptUsingSameKey(this->txFrameBuffer, frameSize);
  auto result = this->radioModule->sendFrame(this->txFrameBuffer, frameSize);
  if (result) {
    this->securityLayerPair->first.lastTxFrameTimestamp =
        AppEnvironment::getEnvironmentContext()->timeBaseMs();
    this->securityLayerPair->first.isAckReceived = false;
  }
}

void WirelessController::sendAck() { this->radioModule->sendRawData(&this->ackByte, 1); }

void WirelessController::processEncryptedFrame(const WirelessFrame& frame, uint8_t frameLength) {
  assert(this->securityLayerPair != nullptr);
  assert(frameLength >= MIN_FRAME_SIZE);

  frame.ToBufferIfEncrypted(this->rxFrameBuffer, frameLength);
  this->securityLayerPair->second->decrypt(this->rxFrameBuffer, frameLength);
}

void WirelessController::processEncryptedFrameUsingPreviousKey(const WirelessFrame& frame,
                                                               uint8_t frameLength) {
  assert(this->securityLayerPair != nullptr);
  frame.ToBufferIfEncrypted(this->rxFrameBuffer, frameLength);
  this->securityLayerPair->second->decryptUsingPreviousKey(this->rxFrameBuffer, frameLength);
}

void WirelessController::securityLayerUpdate() {
  assert(this->securityLayerPair != nullptr);
  if (this->securityLayerPair->second == nullptr) {
    return;
  }
}

bool WirelessController::isAckWaitingTimePassed() {
  assert(this->securityLayerPair != nullptr);

  if (AppEnvironment::getEnvironmentContext()->timeBaseMs() -
          this->securityLayerPair->first.lastTxFrameTimestamp >
      100) {
    return true;
  } else {
    return false;
  }
}

void WirelessController::onAckReceived() {
  assert(this->securityLayerPair != nullptr);
  this->securityLayerPair->first.isAckReceived = true;
}

void WirelessController::onService() {
  if (this->radioModule->isAckAvailable()) {
    this->onAckReceived();
  }
  if (this->isFrameAvailable()) {
    uint8_t frameLength = 0;
    auto& receivedFrame = this->getReceivedFrame(&frameLength);
    assert(this->securityLayerPair != nullptr);

    if (this->securityLayerPair->second) {
      this->processEncryptedFrame(receivedFrame, frameLength);

      WirelessFrame processedFrame;
      processedFrame.ToFrame(this->rxFrameBuffer, frameLength);
      if (!this->checkFrameCRC16(processedFrame)) {  // if crc16 is not valid
        this->processEncryptedFrameUsingPreviousKey(receivedFrame, frameLength);
        processedFrame.ToFrame(this->rxFrameBuffer, frameLength);
        if (this->checkFrameCRC16(processedFrame)) {
          this->sendAck();
        }
      } else {
        this->onCommandService(processedFrame);
        this->securityLayerPair->second->updateKeys();
        this->sendAck();
      }
      return;
    }
    if (!this->checkFrameCRC16(receivedFrame)) {
      return;
    }
    this->onCommandService(receivedFrame);
  }
}

void WirelessController::onCommandService(const WirelessFrame& rxFrame) {
  switch (rxFrame.command) {
    case WirelessCommand::STEERING_DATA:

      break;

    case WirelessCommand::SYNCHRONIZE_RTC:
      this->sendSynchronizeRTCResponse();
      break;
  }
}

void WirelessController::sendSteeringDataRequest(void* data, size_t size) {
  if (size > MAX_WIRELESS_FRAME_DATA_SIZE) {
    return;
  }
  this->send(WirelessCommand::STEERING_DATA, size, data);
}

void WirelessController::sendSynchronizeRTCResponse() {
  static uint64_t lastSynchronizeRequestTs = 0;
  if (AppEnvironment::getEnvironmentContext()->timeBaseMs() - lastSynchronizeRequestTs > 3000) {
    lastSynchronizeRequestTs = AppEnvironment::getEnvironmentContext()->timeBaseMs();
    time_t time = AppEnvironment::getEnvironmentContext()->getUnixTimeFromRTC();

    this->send(WirelessCommand::SYNCHRONIZE_RTC, sizeof(time), &time);
  }
}

bool WirelessController::isFrameAvailable() { return this->radioModule->isFrameAvailable(); }

bool WirelessController::checkFrameCRC16(const WirelessFrame& frame) {
  uint8_t buffer[255];
  size_t frameSize = frame.ToBuffer(buffer, 50);
  if (!frameSize) {
    return false;
  }

  uint16_t calculatedCRC16 = this->calculateCRC16(buffer, frameSize - CRC_SIZE);
  uint16_t receivedCrc16 = 0;
  memcpy(&receivedCrc16, &buffer[frameSize - CRC_SIZE], CRC_SIZE);

  if (calculatedCRC16 == receivedCrc16) {
    return true;
  } else {
    return false;
  }
}

const WirelessFrame& WirelessController::getReceivedFrame(uint8_t* frameLength) {
  return this->radioModule->getReceivedFrame(frameLength);
}

uint16_t WirelessController::calculateCRC16(const uint8_t* data, uint8_t length) {
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
