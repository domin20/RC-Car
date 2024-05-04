#include "Service/EncoderService.h"

uint64_t EncoderService::ticks = 0;

void EncoderService::incrementTicks() { ticks++; }

uint64_t EncoderService::getTicks() { return ticks; }
