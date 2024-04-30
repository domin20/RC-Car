#pragma once

#include <functional>
#include <memory>
#include <vector>

// TODO(Dominik): Split it into separate files and create library

class Integral {
 public:
  explicit Integral(std::function<uint64_t()> timeBase = {});
  explicit Integral(float timeConstant, std::function<uint64_t()> timeBase = {});

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

  std::function<uint64_t()> timeBase;
  static constexpr float INVERSE_EULER = 0.368f;  // inverse euler
};

class Derivative {
 public:
  explicit Derivative(std::function<uint64_t()> timeBase = {});
  explicit Derivative(float timeConstant, std::function<uint64_t()> timeBase = {});

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

  std::function<uint64_t()> timeBase;
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
  explicit PID(std::function<uint64_t()> timeBase = {});
  explicit PID(float propotionalGain, std::function<uint64_t()> timeBase = {});
  PID(float propotionalGain, float integralGain, std::function<uint64_t()> timeBase = {});
  PID(float propotionalGain, float integralGain, float derivativeGain,
      std::function<uint64_t()> timeBase = {});
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

  std::function<uint64_t()> timeBase;
  Integral integral = Integral(timeBase);
  Derivative derivative = Derivative(timeBase);
};
