#include "PID.h"

#include <math.h>

PID::PID(std::function<uint64_t()> timeBase)
    : propotionalGain(1.0f), integralGain(1.0f), derivativeGain(1.0f), timeBase(timeBase) {}

PID::PID(float propotionalGain, std::function<uint64_t()> timeBase)
    : propotionalGain(propotionalGain),
      integralGain(1.0f),
      derivativeGain(1.0f),
      timeBase(timeBase) {}

PID::PID(float propotionalGain, float integralGain, std::function<uint64_t()> timeBase)
    : propotionalGain(propotionalGain),
      integralGain(integralGain),
      derivativeGain(1.0f),
      timeBase(timeBase) {}

PID::PID(float propotionalGain, float integralGain, float derivativeGain,
         std::function<uint64_t()> timeBase)
    : propotionalGain(propotionalGain),
      integralGain(integralGain),
      derivativeGain(derivativeGain),
      timeBase(timeBase) {}

void PID::pidInit(float propotionalGain, float integralGain, float derivativeGain) {
  this->propotionalGain = propotionalGain;
  this->integralGain = integralGain;
  this->derivativeGain = derivativeGain;
}

void PID::setIntegralTimeConstant(float timeConstant) {
  this->integral.setTimeConstant(timeConstant);
}

void PID::setDerivativeTimeConstant(float timeConstant) {
  this->derivative.setTimeConstant(timeConstant);
}

float PID::pidUpdate(float error) {
  float controlSignal;

  // variables for integral term ..
  static float inputValueIt = 0.0f;
  static float outputValueIt = 0.0f;

  // variables for derivative term..
  static float inputValueDt = 0.0f;
  static float outputValueDt = 0.0f;

  // ******************* INTEGRAL TERM *********************
  inputValueIt = error * integralGain;
  outputValueIt = integralUpdate(inputValueIt);

  // ******************* DERIVATIVE TERM *********************
  inputValueDt = error * derivativeGain;
  outputValueDt = derivativeUpdate(inputValueDt);

  controlSignal = propotionalGain * error + outputValueIt + outputValueDt;
  return controlSignal;
}

float PID::integralUpdate(float inputValue) { return this->integral.update(inputValue); }

float PID::derivativeUpdate(float inputValue) { return this->derivative.update(inputValue); }

Integral::Integral(std::function<uint64_t()> timeBase)
    : timeConstant(1.0f),
      rawValueDifference(0.0f),
      processedValueDifference(0.0f),
      outputValue(0.0f),
      exponent(0.0f),
      isRising(true),
      previousTimeStamp(0),
      timeBase(timeBase) {}

Integral::Integral(float timeConstant, std::function<uint64_t()> timeBase)
    : timeConstant(timeConstant),
      rawValueDifference(0.0f),
      processedValueDifference(0.0f),
      outputValue(0.0f),
      exponent(0.0f),
      isRising(true),
      previousTimeStamp(0),
      timeBase(timeBase) {}

float Integral::update(float rawInputValue) {
  exponent = (timeBase() - previousTimeStamp) / (timeConstant * 1000.0f);
  previousTimeStamp = timeBase();
  if (rawInputValue >= this->outputValue) {
    this->isRising = true;
  } else {
    this->isRising = false;
  }

  if (this->isRising) {
    this->rawValueDifference = rawInputValue - this->outputValue;
    this->processedValueDifference =
        this->rawValueDifference * (1.0f - pow(INVERSE_EULER, this->exponent));
    this->outputValue += this->processedValueDifference;
  } else {
    this->rawValueDifference = this->outputValue - rawInputValue;
    this->processedValueDifference =
        this->rawValueDifference * (1.0f - pow(INVERSE_EULER, this->exponent));
    this->outputValue -= this->processedValueDifference;
  }
  return this->outputValue;
}

void Integral::setTimeConstant(float timeConstant) { this->timeConstant = timeConstant; }

Derivative::Derivative(std::function<uint64_t()> timeBase)
    : timeConstant(1.0f),
      rawValueDifference(0.0f),
      processedValueDifference(0.0f),
      outputValue(0.0f),
      exponent(0.0f),
      previousTimeStamp(0),
      timeBase(timeBase) {}

Derivative::Derivative(float timeConstant, std::function<uint64_t()> timeBase)
    : timeConstant(timeConstant),
      rawValueDifference(0.0f),
      processedValueDifference(0.0f),
      outputValue(0.0f),
      exponent(0.0f),
      previousTimeStamp(0),
      timeBase(timeBase) {}

float Derivative::update(float rawInputValue) {
  exponent = (timeBase() - previousTimeStamp) / (this->timeConstant * 1000.0f);

  previousTimeStamp = timeBase();
  difference = rawInputValue - previousRawInputValue;
  outputValue += difference;

  if (outputValue >= 0.0f) {
    processedValueDifference = outputValue * (1.0f - pow(inverseEuler, exponent));
    outputValue -= processedValueDifference;
  } else {
    processedValueDifference = rawValueDifference * (1.0f - pow(inverseEuler, exponent));
    outputValue += processedValueDifference;
  }
  previousRawInputValue = rawInputValue;
  return outputValue;
}

void Derivative::setTimeConstant(float timeConstant) { this->timeConstant = timeConstant; }

void Inertia::setTimeConstant(float timeConstant) {
  for (auto& integral : integralTerms) {
    integral.setTimeConstant(timeConstant);
  }
}

void Inertia::setOrderOfInertia(uint8_t orderOfInertia) {
  for (uint8_t i = 0; i < orderOfInertia; i++) {
    this->integralTerms.emplace_back();
  }
}

float Inertia::update(float rawInputValue) {
  float inputValue = rawInputValue;
  for (auto& integral : this->integralTerms) {
    inputValue = integral.update(inputValue);
  }
  return inputValue;
}
