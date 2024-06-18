#include "SecurityLayerRegistry.h"

etl::unordered_map<SecurityLayerType, SecurityLayerPair, 5> SecurityLayerRegistry::layers;

SecurityLayerContext SecurityLayerRegistry::defaultContext;
SecurityLayerContext SecurityLayerRegistry::xorKeyRotationContext;
SecurityLayerContext SecurityLayerRegistry::timestampContext;

XorKeyRotation SecurityLayerRegistry::xorKeyRotationLayer;
Timestamp SecurityLayerRegistry::timestampLayer;

void SecurityLayerRegistry::init() {
  layers[SecurityLayerType::NONE] = std::make_pair(defaultContext, nullptr);
  layers[SecurityLayerType::XOR_KEY_ROTATION] =
      std::make_pair(xorKeyRotationContext, &xorKeyRotationLayer);

  timestampContext.isAckEnabled = false;
  layers[SecurityLayerType::TIMESTAMP] = std::make_pair(timestampContext, &timestampLayer);
}
