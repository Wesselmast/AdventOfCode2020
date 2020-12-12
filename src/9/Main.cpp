#include <stdio.h>
#include <cstdint>
#include <fstream>

#include "Timer.cpp"

typedef int32_t int32;
typedef uint64_t uint64;

inline bool check_for_bad_preamble(uint64* values, int32 index, uint64 number, const int32 preamble) {
  for(int32 i = index - preamble; i < index; ++i) {
    for(int32 j = index - preamble; j < index; ++j) {
      if(i == j) continue;
      if(values[i] + values[j] == number) return false;
    }
  }
  return true;
}

uint64 find_encryption_weakness(uint64* values, int32 index, uint64 number) {
  uint64 smallest, largest, sum = 0;

  for(int32 i = 0; i < index; ++i) {
    uint64 sum = values[i];
    uint64 smallest = sum;
    uint64 largest  = sum;

    for(int32 j = i + 1; sum < number; ++j) {
      uint64& v = values[j];

      smallest = v < smallest ? v : smallest; 
      largest  = v > largest  ? v : largest; 
      sum += v;

      if(sum == number) return smallest + largest;
    }
  }
  
  return 0;
}

int main() {
  TIMER("Solution total time");
  
  std::ifstream file("res/9/data.txt", std::ios::in);
  if(!file.good()) {
    printf("file wasn't found\n");
    return 1;
  }

  uint64 values[1024] = {0};
  uint64 invalid, weakness;
  int32 index = 0;
  const int32 preamble = 25;

  while(file >> values[index]) {
    if(index >= preamble) {
       if(check_for_bad_preamble(values, index, values[index], preamble)) {
	 invalid = values[index];
	 weakness = find_encryption_weakness(values, index, invalid);
	 break;
       }
    }
    ++index;
  }

  printf("number:   %llu\n", invalid);
  printf("weakness: %llu\n", weakness);

  return 0;
}
