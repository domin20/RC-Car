#pragma once

#include <memory>
#include <vector>

#include "Types.h"

// TODO(Dominik): Split it into separate files and create library

class Integral {
 public:
  explicit Integral(Function<uint64_t()> timeBase = nullptr);
  explicit Integral(float timeConstant, Function<uint64_t()> timeBase = nullptr);

  float update(float rawInputValue);
  void setTimeConstant(float timeConstant);

 private:
  float timeConstant;
  float rawValueDifference;
  float processedValueDifference;
  float outputValue;
  float exponent;

  bool isRising;
  uint64_t previousTimeStamp;

  Function<uint64_t()> timeBase;
  static constexpr float INVERSE_EULER = 0.368f;  // inverse euler
};

class Derivative {
 public:
  explicit Derivative(Function<uint64_t()> timeBase = nullptr);
  explicit Derivative(float timeConstant, Function<uint64_t()> timeBase = nullptr);

  float update(float rawInputValue);
  void setTimeConstant(float timeConstant);

 private:
  float timeConstant;
  static const float inverseEuler;  // inverse euler
  float rawValueDifference;
  float processedValueDifference;
  float previousRawInputValue;
  float outputValue;
  float exponent;
  float difference;

  uint64_t previousTimeStamp;

  Function<uint64_t()> timeBase = nullptr;
};

class Inertia {
 public:
  Inertia() {}
  ~Inertia() {}

  void setTimeConstant(float timeConstant);
  void setOrderOfInertia(uint8_t orderOfInertia);
  float update(float rawInputValue);

 private:
  std::vector<Integral> integralTerms;
};

class PID {
 public:
  explicit PID(Function<uint64_t()> timeBase = nullptr);
  explicit PID(float propotionalGain, Function<uint64_t()> timeBase = nullptr);
  PID(float propotionalGain, float integralGain, Function<uint64_t()> timeBase = nullptr);
  PID(float propotionalGain, float integralGain, float derivativeGain,
      Function<uint64_t()> timeBase = nullptr);
  ~PID(){};

  void setIntegralTimeConstant(float timeConstant);
  void setDerivativeTimeConstant(float timeConstant);
  void pidInit(float propotionalGain, float integralGain, float derivativeGain);

  float pidUpdate(float error);

 private:
  float integralUpdate(float inuptValue);
  float derivativeUpdate(float inputValue);

 private:
  float propotionalGain;
  float integralGain;
  float derivativeGain;

  Function<uint64_t()> timeBase = nullptr;
  Integral integral = Integral(timeBase);
  Derivative derivative = Derivative(timeBase);
};
