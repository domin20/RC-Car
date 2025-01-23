#pragma once
#include "HC12Module.h"
#include "SecurityLayers/BasicSecurityLayer.h"

class WirelessController {
 public:
  void init(HC12Module* module);
  void setSecurityLayer(SecurityLayerPair& layerPair) { this->securityLayerPair = &layerPair; }
  void onService();

  // Requests
  void sendSteeringDataRequest(void* data, size_t size);

  // Reponses
  void sendSynchronizeRTCResponse();

  bool isFrameAvailable();
  bool checkFrameCRC16(const WirelessFrame& frame, uint8_t frameLength, bool isEncrypted);
  const WirelessFrame& getReceivedFrame(uint8_t* frameLength = nullptr);

 private:
  void send(uint8_t command, void* data, uint8_t dataSize);
  void sendEncryptedFrame(uint8_t frameSize);
  void sendEncryptedFrameUsingSameKey(uint8_t frameSize);
  void sendAck();

  bool processEncryptedFrame(const WirelessFrame& frame, uint8_t frameLength);
  bool processEncryptedFrameUsingPreviousKey(const WirelessFrame& frame, uint8_t frameLength);

  void securityLayerUpdate();
  bool isAckWaitingTimePassed();
  void onAckReceived();

  uint16_t calculateCRC16(const uint8_t* data, uint8_t length);
  void onCommandService(const WirelessFrame& rxFrame);

 private:
  uint8_t ackByte = ACK_BYTE;
  uint8_t txFrameBuffer[MAX_FRAME_SIZE] = {0};
  uint8_t rxFrameBuffer[MAX_FRAME_SIZE] = {0};
  WirelessFrame txFrame;
  HC12Module* radioModule;

  SecurityLayerPair* securityLayerPair;
};
