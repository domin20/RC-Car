#pragma once

#include <etl/unordered_map.h>

#include "Wireless/SecurityLayers/RotatingKey.h"
#include "Wireless/SecurityLayers/Timestamp.h"

enum SecurityLayerType : uint8_t { NONE, ROTATING_KEY, TIMESTAMP };

class SecurityLayerRegistry {
 public:
  static void init();
  static BasicSecurityLayer* getRotatingKeyLayer() { return &rotatingKeyLayer; }
  static BasicSecurityLayer* getTimestampLayer() { return &timestampLayer; }
  static SecurityLayerPair& getSecurityLayer(SecurityLayerType type) {
    assert(layers.find(type) != layers.end());
    return layers[type];
  }

 private:
  static etl::unordered_map<SecurityLayerType, SecurityLayerPair, 5> layers;

  static SecurityLayerContext defaultContext;
  static SecurityLayerContext rotatingKeyContext;
  static SecurityLayerContext timestampContext;

  static RotatingKey rotatingKeyLayer;
  static Timestamp timestampLayer;
};
