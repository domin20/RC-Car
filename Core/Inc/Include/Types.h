#pragma once

enum DriveDirection { FORWARD = 0, REVERSE = 1 };
enum TurnDirection { LEFT = 0, RIGHT = 1 };

template <typename ReturnType, typename... Args>
class Function;

template <typename ReturnType, typename... Args>
class Function<ReturnType(Args...)> {
 public:
  Function(ReturnType (*func)(Args...)) : func(func) {}
  ReturnType operator()(Args... args) const { return func(args...); }
  explicit operator bool() const { return func != nullptr; }

 private:
  ReturnType (*func)(Args...);
};
