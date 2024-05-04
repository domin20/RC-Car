#include "Wireless/WirelessController.h"

#include <string.h>

#include "App.h"

void WirelessController::init(HC12Module* module) { this->radioModule = module; }

void WirelessController::onService() {
  if (this->isFrameAvailable()) {
    auto& receivedFrame = this->getReceivedFrame();

    switch (receivedFrame.command) {
      case WirelessCommand::STEERING_DATA:
        SteeringData steeringData;
        if (receivedFrame.dataSize != sizeof(steeringData)) {
          return;
        } else {
          memcpy(&steeringData, receivedFrame.data, receivedFrame.dataSize);
          App::performSteeringData(steeringData);
        }
        break;
    }
  }
}

bool WirelessController::isFrameAvailable() { return this->radioModule->isFrameAvailable(); }

const WirelessFrame& WirelessController::getReceivedFrame() {
  return this->radioModule->getReceivedFrame();
}
