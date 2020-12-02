#pragma once

#include <chrono>

#define TIMER(str) Timer awd79h3r923hgiufbd9sher2nf20hbdsga0wd(str)

struct Timer {
  Timer(const char* name) : name(name) {
    start = timer.now();
  }

  ~Timer() {
    printf("%s : %fs\n", name, std::chrono::duration<double>(timer.now() - start).count());
  }

  const char* name;

  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::high_resolution_clock timer;
};
