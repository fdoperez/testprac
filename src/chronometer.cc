#include "chronometer.h"

void Chronometer::start()  {
  start_ = std::chrono::high_resolution_clock::now();
  running_ = true;
}

void Chronometer::stop() {
  if (running_) {
    end_ = std::chrono::high_resolution_clock::now();
    running_ = false;
  }
}

void Chronometer::restart() {
  running_ = false;
}

long long Chronometer::get_time() const {
  if (running_) {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_).count();
  } else {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end_ - start_).count();
  }
}
