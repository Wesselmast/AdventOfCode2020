#include <stdio.h>
#include <fstream>
#include <cstdint>
#include <math.h>
#include <vector>
#include <algorithm>

#include "Timer.cpp"

typedef int32_t int32;
typedef uint8_t uint8;

inline int32 count_bits(int32 num) {
  int32 result = 0;
  while(num) {
    result += num & 1;
    num >>= 1;
  }
  return result;
}

inline void mark_answered_yes(int32& answers, char answer) {
  answers |= 1 << ((int32)answer - 97);
}

inline void mask_answered_yes(int32& answers, int32 mask) {
  answers &= mask;
}

int main() {
  TIMER("Solution total time");
  
  FILE* file = fopen("res/6/data.txt", "r");
  if(!file) {
    printf("file wasn't found\n");
    return 1;
  }

  int32 result1 = 0;
  int32 result2 = 0;
  int32 answers1 = 0;
  int32 answers2 = ~0;

  char* line = (char*)malloc(512);
  while(true) {
    char* r = fgets(line, 512, file);
    int32 len = strlen(line) - 1;
    line[len] = 0;

    if(!len || !r) {
      {
	int32 bits = count_bits(answers1);
	result1 += bits;
	answers1 = 0;
      }
      {
	int32 bits = count_bits(answers2);
	result2 += bits;
	answers2 = ~0;
      }
      if(!r) break;
      continue;
    }

    int32 personCount = 0;
    for(uint8 i = 0; i < len; ++i) {
      mark_answered_yes(answers1, line[i]);
      mark_answered_yes(personCount, line[i]);
    }
    mask_answered_yes(answers2, personCount);
  }

  printf("P1: %d\n", result1);
  printf("P2: %d\n", result2);

  free(line);
  fclose(file);
  return 0;
}
