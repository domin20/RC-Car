#pragma once

#include "../Types.h"
#include "Structures.h"
#include "Types.h"
#include "usart.h"

class HC12Module {
 public:
  void init(UART_HandleTypeDef* huart, Function<uint64_t()> timeBaseUs = nullptr);

  void sendFrame(const WirelessFrame& frame);
  const WirelessFrame& getReceivedFrame();
  bool isFrameAvailable();

  void update();

  void setTxCpltFlag();

 private:
  void onReceivedData(void);

  void processFrame();
  void loadDataToFrameStruct();

  uint16_t calculateCRC16(uint8_t* data, uint8_t length);

 private:
  UART_HandleTypeDef* huart;

  WirelessFrame receivedFrame;
  ReceiverContext receiverContext;
  volatile uint64_t timeStampLastByte;
  bool isFrameReceiving;
  bool isTxCplt;

  Function<uint64_t()> timeBaseUs = nullptr;
};