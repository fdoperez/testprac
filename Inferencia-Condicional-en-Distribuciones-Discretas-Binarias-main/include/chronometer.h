#ifndef CHRONOMETER_H
#define CHRONOMETER_H

#include <chrono>
#include <string>
#include <iostream>

class Chronometer {
 public:
  Chronometer() : running_(false) {}
  void start();
  void stop();
  void restart();
  long long get_time() const;
 private:
  std::chrono::high_resolution_clock::time_point start_;
  std::chrono::high_resolution_clock::time_point end_;
  bool running_;
};

#endif  // CHRONOMETER_H
