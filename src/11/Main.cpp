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

void day_11() {
  TIMER("Solution total time");
  
  std::ifstream file("res/11/data.txt", std::ios::in);
  if(!file.good()) {
    printf("file wasn't found\n");
    return;
  }
 
  int32 values[128][4] = {0};
  int32 seatMask[128][4] = {0};

  char line[128];
  int32 index = 1;
  while(file >> line) {
    int32 len = strlen(line);
    int32 depth = -1;
    for(int32 i = 0; i < len; ++i) {
      depth += i % 32 == 0;
      if(line[i] != '.') {
	seatMask[index][depth] |= 1 << ((i + 1) % 32);    //something here is not quite right!
      }
    }
    ++index;
  }

  for(int32 i = 3; i >= 0; --i) {
    print_as_binary(seatMask[5][i]);
  }
  printf("\n");
}


//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
//  start_application(hInstance, nCmdShow, day_11, {0.1f, 0.1f, 0.1f});
//}
//

int main() {
  day_11();
  return 0;
}
