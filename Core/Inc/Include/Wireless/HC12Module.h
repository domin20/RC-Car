#pragma once

#include "Structures.h"
#include "Types.h"

class HC12Module {
 public:
  void init(Function<uint64_t()> timeBase);

  void sendFrame(const WirelessFrame& frame);
  const WirelessFrame& getReceivedFrame();

  bool isFrameAvailable();

  void update();

 private:
  void performFrame();
  void loadDataToFrame();

  uint16_t calculateCRC16(uint8_t* data, uint8_t length);

 private:
  WirelessFrame rxFrame;
  volatile uint64_t timeStampLastByte;
  bool isFrameReceiving;

  Function<uint64_t()> timeBase;
};