#include <stdio.h>
#include <fstream>

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

inline uint32 count_neighbours_king(int32 col, int32 row, int32 (&values)[128][4], int32 maxValue) {
  uint32 result = 0;
  for(int32 i = col - 1; i <= col + 1; ++i) {
    if(i < 0 || i >= maxValue) continue;
    for(int32 j = row - 1; j <= row + 1; ++j) {
      if(j < 0 || j >= maxValue || (i == col && j == row)) continue;
      result += check_bit(values[i][get_depth(j)], j % 32); 
    }
  } 
  return result;
}

inline uint32 count_neighbours_queen(int32 col, int32 row, 
				     int32 (&values)[128][4], 
				     int32 (&seatMask)[128][4], 
				     int32 maxValue) {
  uint32 result = 0;

  for(int32 i = col - 1; i <= col + 1; ++i) {
    if(i < 0 || i >= maxValue) continue;
    for(int32 j = row - 1; j <= row + 1; ++j) {
      if(j < 0 || j >= maxValue || (i == col && j == row)) continue;
      int32 jD = get_depth(j);
      int32 jMod = j % 32;

      if(check_bit(seatMask[i][jD], jMod)) {
	result += check_bit(values[i][jD], jMod);
	continue;
      }

      int32 iC = i;
      int32 jC = j;

      while(true) {
	iC += i - col;
	jC += j - row;
	if(iC < 0 || iC >= maxValue || jC < 0 || jC >= maxValue) break;
	
	int32 jDC = get_depth(jC);
	int32 jModC = jC % 32;

	if(check_bit(seatMask[iC][jDC], jModC)) {
	  result += check_bit(values[iC][jDC], jModC);
	  break;
	}
      }
    }
  } 

  return result;
}

uint32 count_seats(int32 (&values)[128][4], int32 maxValue) {
  uint32 result = 0;
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

void draw_seats(int32 boardOffset, int32 (&values)[128][4], int32 maxValue, 
		const glm::vec3& col1, const glm::vec3& col2) {
  const int32 offset = 40;
  const int32 size = 18;
  const int32 midX = (w / 2) - boardOffset;
  const int32 midY = (h / 2);

  for(int32 i = 0; i < maxValue; ++i) {
    for(int32 j = 0; j < maxValue; ++j) {
      if(check_bit(values[i][get_depth(j)], j % 32)) {
	triangle(((i - maxValue * 0.5) * offset) + midX, ((j - maxValue * 0.5f) * offset) + midY, size, col1);
	continue;
      }
      rect(((i - maxValue * 0.5f) * offset) + midX, ((j - maxValue * 0.5f) * offset) + midY, size, col2);
    }
  }

}

void day_11() {
  TIMER("Solution total time");
  
  std::ifstream file("res/11/data.txt", std::ios::in);
  if(!file.good()) {
    printf("file wasn't found\n");
    return;
  }
 
  int32 values1[128][4] = {0};
  int32 next1[128][4] = {0};
  int32 values2[128][4] = {0};
  int32 next2[128][4] = {0};
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
  index++;

  bool changed = true;

  while(changed) {
    changed = false;
    for(int32 i = 0; i < index; ++i) {
      for(int32 j = 0; j < index; ++j) {
	int32 jMod = j % 32;
        int32 jD = get_depth(j);

        if(!check_bit(seatMask[i][jD], jMod)) continue;

	bool changed1, changed2;

	{
	  int32 c = values1[i][jD];
	  int32& n = next1[i][jD];
	  int32 neighbours = count_neighbours_king(i, j, values1, index);

	  bool cBit = check_bit(c, jMod);
	  if(cBit) {
	    if(neighbours >= 4) {
	      clear_bit(n, jMod);
	    }
	  }
	  else if(!neighbours) {
	    set_bit(n, jMod);
	  }
	  
	  changed1 = cBit != check_bit(n, jMod);
	}

	{
	  int32 c = values2[i][jD];
	  int32& n = next2[i][jD];
	  int32 neighbours = count_neighbours_queen(i, j, values2, seatMask, index);

	  bool cBit = check_bit(c, jMod);
	  if(cBit) {
	    if(neighbours >= 5) {
	      clear_bit(n, jMod);
	    }
	  }
	  else if(!neighbours) {
	    set_bit(n, jMod);
	  }
	  changed2 = cBit != check_bit(n, jMod);
	}

	changed |= changed1 || changed2;
      }
    }
    memcpy(&values1, &next1, sizeof(next1));
    memcpy(&values2, &next2, sizeof(next2));
  }

  //these values are correct, don't have text rendering yet
  printf("Puzzle 1 seats: %d\n", count_seats(values1, index));
  printf("Puzzle 2 seats: %d\n", count_seats(values2, index));
  
  const glm::vec3 col1 = {209.0f/255.0f, 64.0f/255.0f,  129.0f/255.0f};
  const glm::vec3 col2 = { 63.0f/255.0f, 136.0f/255.0f, 197.0f/255.0f};
  const glm::vec3 col3 = {255.0f/255.0f, 210.0f/255.0f, 117.0f/255.0f};
  const glm::vec3 col4 = {104.0f/255.0f, 182.0f/255.0f, 132.0f/255.0f};

  draw_seats( 300, values1, index, col1, col2);
  draw_seats(-300, values2, index, col3, col4);
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
  start_application(hInstance, nCmdShow, day_11, {0.1f, 0.1f, 0.1f});
  return 0;
}

