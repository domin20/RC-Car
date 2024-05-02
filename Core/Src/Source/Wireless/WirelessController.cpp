#include "Wireless/WirelessController.h"

void WirelessController::init(HC12Module& module) { this->radioModule = module; }

void WirelessController::onService() {
  if (this->isFrameAvailable()) {
    auto& receivedFrame = this->getReceivedFrame();

    switch (receivedFrame.command) {}
  }
}

bool WirelessController::isFrameAvailable() { return this->radioModule.isFrameAvailable(); }

const WirelessFrame& WirelessController::getReceivedFrame() {
  return this->radioModule.getReceivedFrame();
}
