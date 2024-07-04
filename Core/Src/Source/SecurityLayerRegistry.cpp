#include "SecurityLayerRegistry.h"

etl::unordered_map<SecurityLayerType, SecurityLayerPair, 5> SecurityLayerRegistry::layers;

SecurityLayerContext SecurityLayerRegistry::defaultContext;
SecurityLayerContext SecurityLayerRegistry::rotatingKeyContext;
SecurityLayerContext SecurityLayerRegistry::timestampContext;

RotatingKey SecurityLayerRegistry::rotatingKeyLayer;
Timestamp SecurityLayerRegistry::timestampLayer;

void SecurityLayerRegistry::init() {
  layers[SecurityLayerType::NONE] = std::make_pair(defaultContext, nullptr);
  layers[SecurityLayerType::ROTATING_KEY] = std::make_pair(rotatingKeyContext, &rotatingKeyLayer);

  timestampContext.isAckEnabled = false;
  layers[SecurityLayerType::TIMESTAMP] = std::make_pair(timestampContext, &timestampLayer);
}
