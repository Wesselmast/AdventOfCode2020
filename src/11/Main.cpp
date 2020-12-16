#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>

#include "Win32.h"
#include "Timer.cpp"
#include "Types.cpp"

#define B2B_PATTERN_8 "%c%c%c%c%c%c%c%c"
#define B2B_8(i)    \
    (((i) & 0x80ll) ? '1' : '0'), \
    (((i) & 0x40ll) ? '1' : '0'), \
    (((i) & 0x20ll) ? '1' : '0'), \
    (((i) & 0x10ll) ? '1' : '0'), \
    (((i) & 0x08ll) ? '1' : '0'), \
    (((i) & 0x04ll) ? '1' : '0'), \
    (((i) & 0x02ll) ? '1' : '0'), \
    (((i) & 0x01ll) ? '1' : '0')

#define B2B_PATTERN_16 B2B_PATTERN_8  B2B_PATTERN_8
#define B2B_PATTERN_32 B2B_PATTERN_16 B2B_PATTERN_16

#define B2B_16(i) B2B_8(i >> 8),   B2B_8(i)
#define B2B_32(i) B2B_16(i >> 16), B2B_16(i)

void print_as_binary(int32 n) {
  printf(B2B_PATTERN_32, B2B_32(n));
}

inline bool check_bit(int32 n, uint8 pos) {
  return n & 1 << pos;
}

inline void set_bit(int32& n, uint8 pos) {
  n |= 1 << pos;
}

inline void clear_bit(int32& n, uint8 pos) {
  n &= ~(1 << pos);
}

inline int32 get_depth(int32 n) {
  int32 depth = 0;
  while(n - 32 >= 0) {
    ++depth;
    n -= 32;
  }
  return depth;
}

inline uint32 count_neighbours(int32 col, int32 row, int32 (&values)[128][4], int32 maxValue) {
  int32 result = 0;
  for(int32 i = col - 1; i <= col + 1; ++i) {
    if(i < 0 || i >= maxValue) continue;
    for(int32 j = row - 1; j <= row + 1; ++j) {
      if(j < 0 || j >= maxValue || (i == col && j == row)) continue;
      result += check_bit(values[i][get_depth(j)], j % 32); 
    }
  } 
  return result;
}

int32 count_seats(int32 (&values)[128][4], int32 maxValue) {
  int32 result = 0;
  for(int32 i = 0; i < maxValue; ++i) {
    for(int32 j = 0; j < maxValue; ++j) {
      result += check_bit(values[i][get_depth(j)], j % 32);
    }
  }
  return result;
}

void print_seats(int32 (&values)[128][4], int32 maxValue) {
  for(int32 i = 0; i < maxValue; ++i) {
    for(int32 j = 3; j >= 0; --j) {
      print_as_binary(values[i][j]);
    }
    printf("\n");
  }
}

void day_11() {
  TIMER("Solution total time");
  
  std::ifstream file("res/11/data.txt", std::ios::in);
  if(!file.good()) {
    printf("file wasn't found\n");
    return;
  }
 
  int32 values[128][4] = {0};
  int32 next[128][4] = {0};
  int32 seatMask[128][4] = {0};

  char line[128];
  int32 index = 0;
  while(file >> line) {
    int32 len = strlen(line);
    int32 depth = -1;
    for(int32 i = 0; i < len; ++i) {
      int32 iMod = i % 32;
      depth += iMod == 0;
      if(line[i] != '.') {
	set_bit(seatMask[index][depth], iMod);
      }
    }
    ++index;
  }

  bool changed = true;
  int32 it = 0;

  while(changed) {
    changed = false;
    for(int32 i = 0; i < index; ++i) {
      for(int32 j = 0; j < index; ++j) {
	int32 jMod = j % 32;
        int32 jD = get_depth(j);

        if(!check_bit(seatMask[i][jD], jMod)) continue;

        int32 c = values[i][jD];
        int32& n = next[i][jD];
        int32 neighbours = count_neighbours(i, j, values, index);

	bool cBit = check_bit(c, jMod);
        if(cBit) {
          if(neighbours >= 4) {
            clear_bit(n, jMod);
          }
        }
        else if(!neighbours) {
          set_bit(n, jMod);
        }

	changed |= cBit != check_bit(n, jMod);
      }
    }
    memcpy(&values, &next, 2048);
    ++it;
  }

  printf("did %d iterations\n", it);
  printf("SEATS: %d\n", count_seats(values, index));
}

//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
//  start_application(hInstance, nCmdShow, day_11, {0.1f, 0.1f, 0.1f});
//}
//

int main() {
  day_11();
  return 0;
}
