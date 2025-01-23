#pragma once
#include <stdint.h>

class EncoderService {
 public:
  static void incrementTicks();
  static uint64_t getTicks();

 private:
  static uint64_t ticks;
};
