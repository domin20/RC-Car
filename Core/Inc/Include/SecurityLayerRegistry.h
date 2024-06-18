#pragma once

#include <etl/unordered_map.h>

#include "Wireless/SecurityLayers/Timestamp.h"
#include "Wireless/SecurityLayers/XorKeyRotation.h"

enum SecurityLayerType : uint8_t { NONE, XOR_KEY_ROTATION, TIMESTAMP };

class SecurityLayerRegistry {
 public:
  static void init();
  static BasicSecurityLayer* getXorKeyRotation() { return &xorKeyRotationLayer; }
  static BasicSecurityLayer* getTimestampLayer() { return &timestampLayer; }
  static SecurityLayerPair& getSecurityLayer(SecurityLayerType type) {
    assert(layers.find(type) != layers.end());
    return layers[type];
  }

 private:
  static etl::unordered_map<SecurityLayerType, SecurityLayerPair, 5> layers;

  static SecurityLayerContext defaultContext;
  static SecurityLayerContext xorKeyRotationContext;
  static SecurityLayerContext timestampContext;

  static XorKeyRotation xorKeyRotationLayer;
  static Timestamp timestampLayer;
};
