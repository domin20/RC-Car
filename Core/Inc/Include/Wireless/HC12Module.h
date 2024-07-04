#pragma once

#include <etl/delegate.h>

#include "../Types.h"
#include "GPIOPortPin.h"
#include "Structures.h"
#include "Types.h"
#include "usart.h"

class HC12Module {
 public:
  void init(UART_HandleTypeDef* huart, GPIOPortPin* setPin, etl::delegate<uint64_t()> timeBaseUs10);

  bool sendFrame(uint8_t* frame, uint8_t size);
  void sendRawData(void* data, size_t size);
  const WirelessFrame& getReceivedFrame(uint8_t* frameLength = nullptr);

  void enableEncryptionProcessing() { this->isEncryptionProcessingEnabled = true; }
  void disableEncryptionProcessing() { this->isEncryptionProcessingEnabled = false; }
  void setByteUsedAsReceivedAck(uint8_t ack) { this->ackByte = ack; }

  bool isFrameAvailable();
  bool isAckAvailable();

  void update();
  void onReceivedData(void);

  void enableNormalMode();
  void enableCommandMode();

  void setTxCpltFlag();

 private:
  void processFrame();
  void loadDataToFrameStruct();

 private:
  UART_HandleTypeDef* huart;
  GPIOPortPin* setPin;

  WirelessFrame receivedFrame;
  uint8_t receivedFrameLength = 0;

  ReceiverContext receiverContext;
  volatile uint64_t timeStampLastByte;
  bool isFrameReceiving;
  bool isTxCplt;
  bool isAckReceived = false;
  bool isEncryptionProcessingEnabled = false;
  uint8_t ackByte;

  etl::delegate<uint64_t()> timeBaseUs10;
};