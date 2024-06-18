#include "Wireless/SecurityLayers/BasicSecurityLayer.h"

#include <random>

std::mt19937 BasicSecurityLayer::generator(std::random_device{}());
