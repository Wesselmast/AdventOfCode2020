#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>

#include "Win32.h"
#include "Timer.cpp"
#include "Types.cpp"

const int32 GRID_COLS = 1024;
const int32 GRID_ROWS = 256;
const int32 GRID_SIZE = GRID_COLS * GRID_ROWS;

inline void count_neighbours(char grid[GRID_COLS][GRID_ROWS], int32 x, int32 y, 
			     int32 colLen, int32 rowLen, int32& res) {
  res = 0;
  for(int32 i = -1; i <= 1; ++i) {
    for(int32 j = -1; j <= 1; ++j) {
      int32 col = (x + i + colLen) % colLen;
      int32 row = (y + j + rowLen) % rowLen;
      res += grid[col][row] == '#';
      printf("%c\n", grid[col][row]);
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

  char grid[GRID_COLS][GRID_ROWS];
  int32 index = 0;
  char line[GRID_ROWS];
  while(file >> line) {
    strcpy(grid[index], line);
    ++index;
  } 

  int32 colLen = index; 
  int32 rowLen = strlen(grid[0]);

  int32 res = 0;
  count_neighbours(grid, 1, 1, colLen, rowLen, res);
  printf("%d\n", res);

  bool changed = false;
  while(changed) {
    char next[GRID_COLS][GRID_ROWS];

    for(int32 i = 0; i < colLen; ++i) {
      for(int32 j = 0; j < rowLen; ++j) {
	char state = grid[i][j];
	if(state == '.') {
	  next[i][j] = '.';
	  continue;
	}
	int32 n = 0;
	count_neighbours(grid, i, j, colLen, rowLen, n);
	if(state == 'L' && n == 0) {
	  next[i][j] = '#';
	  changed = true;
	  continue;
	}
	if(state == '#' && n > 3) {
	  next[i][j] = 'L';
	  changed = true;
	  continue;
	}
	next[i][j] = state;
      }
    }
//    for(int32 i = 0; i < colLen; ++i) {
//      for(int32 j = 0; j < rowLen; ++j) {
//	printf("%c", next[i][j]);
//      }
//      printf("\n");
//    }
	 
    memcpy(grid, next, GRID_SIZE);
  }
}


//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
//  start_application(hInstance, nCmdShow, day_11, {0.1f, 0.1f, 0.1f});
//}
//

int main() {
  day_11();
  return 0;
}
