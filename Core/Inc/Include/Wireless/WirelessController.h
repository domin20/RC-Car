#pragma once
#include "HC12Module.h"

class WirelessController {
 public:
  void init(HC12Module& module);
  void onService();

  bool isFrameAvailable();
  const WirelessFrame& getReceivedFrame();

 private:
  HC12Module& radioModule;
};
