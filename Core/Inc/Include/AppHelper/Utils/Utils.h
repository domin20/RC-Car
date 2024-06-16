#pragma once
#include <cmath>

#include "AppHelper/Env/AppEnvironment.h"

void __assert_func(const char*, int, const char*, const char*) {
  AppEnvironment::getEnvironmentContext()->assertFailed();
}

int map(int value, int inMin, int inMax, int outMin, int outMax) {
  if (value < inMin) {
    value = inMin;
  } else if (value > inMax) {
    value = inMax;
  }

  // Obliczenie zakresów jako zmiennych zmiennoprzecinkowych
  float inRange = static_cast<float>(inMax - inMin);
  float outRange = static_cast<float>(outMax - outMin);
  float ratio = static_cast<float>(value - inMin) / inRange;

  // Przekształcenie wartości wyjściowej
  int result = static_cast<int>(round(outMin + ratio * outRange));

  // Jeśli zakres wyjściowy jest odwrotny
  if (outMin > outMax) {
    result = outMin - ratio * (outMin - outMax);
  } else {
    result = outMin + ratio * (outMax - outMin);
  }

  return result;
}

void convertToASCIIBuffer(char* buffer, size_t bufferSize, int value) {
  int numDigits = 0;
  int tempValue = value;
  do {
    numDigits++;
    tempValue /= 10;
  } while (tempValue > 0);

  for (size_t i = 0; i < bufferSize; ++i) {
    buffer[i] = ' ';
  }

  for (int i = numDigits - 1; i >= 0; --i) {
    buffer[i] = (value % 10) + '0';
    value /= 10;
  }
}
