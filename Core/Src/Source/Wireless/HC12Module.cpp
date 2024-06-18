#include "Wireless/HC12Module.h"

#include <main.h>
#include <string.h>

void HC12Module::init(UART_HandleTypeDef* huart, GPIOPortPin* setPin,
                      etl::delegate<uint64_t()> timeBaseUs) {
  this->huart = huart;
  this->timeBaseUs = timeBaseUs;
  this->setPin = setPin;
  this->enableNormalMode();

  HAL_UART_Receive_IT(huart, (uint8_t*)&receiverContext.rxByte, 1);
}

bool HC12Module::sendFrame(uint8_t* frame, uint8_t size) {
  static uint8_t frameBuffer[MAX_FRAME_SIZE];
  if (size > MAX_FRAME_SIZE) {
    return false;
  }

  memcpy(frameBuffer, frame, size);
  this->isTxCplt = false;
  HAL_UART_Transmit_IT(this->huart, frameBuffer, size);
  return true;
}

void HC12Module::sendRawData(void* data, size_t size) {
  HAL_UART_Transmit_IT(this->huart, (uint8_t*)data, size);
}

const WirelessFrame& HC12Module::getReceivedFrame(uint8_t* frameLength) {
  if (frameLength != nullptr) {
    *frameLength = this->receivedFrameLength;
  }
  this->receiverContext.isRxCompleted = false;
  return this->receivedFrame;
}

bool HC12Module::isFrameAvailable() { return this->receiverContext.isRxCompleted; }

bool HC12Module::isAckAvailable() {
  bool r = this->isAckReceived;
  this->isAckReceived = false;
  return r;
}

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
  uint8_t frameIdx = 0;

  this->receivedFrame.master = this->receiverContext.rxFrame[frameIdx++];
  this->receivedFrame.command = this->receiverContext.rxFrame[frameIdx++];
  this->receivedFrame.dataSize = this->receiverContext.rxFrame[frameIdx++];

  uint8_t tempDataSize = 0;
  if (this->isEncryptionProcessingEnabled) {
    if (this->receiverContext.rxFrameSize < MIN_FRAME_SIZE + FRAME_KEY_SIZE) {
      return;
    }
    tempDataSize = this->receiverContext.rxFrameSize - MIN_FRAME_SIZE - FRAME_KEY_SIZE;
  } else {
    tempDataSize = this->receiverContext.rxFrameSize - MIN_FRAME_SIZE;
  }

  memcpy(this->receivedFrame.data, (uint8_t*)&this->receiverContext.rxFrame[frameIdx],
         tempDataSize);
  frameIdx += tempDataSize;

  memcpy(&this->receivedFrame.crc16, (uint8_t*)&this->receiverContext.rxFrame[frameIdx],
         sizeof(this->receivedFrame.crc16));
  frameIdx += sizeof(this->receivedFrame.crc16);

  if (this->isEncryptionProcessingEnabled) {
    memcpy(&this->receivedFrame.key, (uint8_t*)&this->receiverContext.rxFrame[frameIdx],
           sizeof(this->receivedFrame.key));
  } else {
    this->receivedFrame.key = 0;
  }

  this->receivedFrameLength = this->receiverContext.rxFrameSize;
  this->receiverContext.isRxCompleted = true;
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

void HC12Module::enableNormalMode() { this->setPin->set(); }

void HC12Module::enableCommandMode() { this->setPin->reset(); }

void HC12Module::processFrame() {
  if (this->receiverContext.rxFrameSize == 1 && this->receiverContext.rxFrame[0] == this->ackByte) {
    this->receiverContext.rxFrame[0] = 0;
    this->isAckReceived = true;
  } else if (this->receiverContext.rxFrameSize >= MIN_FRAME_SIZE) {
    this->loadDataToFrameStruct();
  }
  this->receiverContext.rxFrameSize = 0;
  this->isFrameReceiving = false;
}
