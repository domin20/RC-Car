#pragma once
#include <cmath>

int map(int value, int inMin, int inMax, int outMin, int outMax) {
  if (value < inMin) {
    value = inMin;
  } else if (value > inMax) {
    value = inMax;
  }

  float inRange = inMax - inMin;
  float outRange = outMax - outMin;
  float ratio = (value - inMin) / inRange;

  if (outMin > outMax) {
    ratio = 1 - ratio;
  }

  int result = round(outMin + ratio * outRange);
  return result;
}
