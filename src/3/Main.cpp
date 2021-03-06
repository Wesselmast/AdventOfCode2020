#include <stdio.h>
#include <fstream>
#include <cstdint>
#include <string.h>

#include "Timer.cpp"
#include "Types.cpp"

inline int32 count_trees(char arr[512][256], int32 cLength, int32 rLength, int32 cOffset, int32 rOffset) {
  int32 traversal = 0;
  int32 result = 0;

  for(int32 i = 0; i < rLength; i += rOffset) {
    result += arr[i][traversal % cLength] == '#';
    traversal += cOffset;
  }
  return result;
}

inline int32 count_multiple_trees(char arr[512][256], int32 cLength, int32 rLength) {
  int32 result = 1;

  result *= count_trees(arr, cLength, rLength, 1, 1);
  result *= count_trees(arr, cLength, rLength, 3, 1);
  result *= count_trees(arr, cLength, rLength, 5, 1);
  result *= count_trees(arr, cLength, rLength, 7, 1);
  result *= count_trees(arr, cLength, rLength, 1, 2);

  return result;
}

int main() {
  TIMER("Total solution time");

  std::ifstream file("res/3/data.txt", std::ios::in);
  if(!file.good()) {
    printf("file wasn't found\n");
    return 1;
  }

  char arr[512][256];
  char line[256];
  int32 index = 1;
  int32 len = 0;

  file >> line;
  strcpy(arr[index], line);
  len = strlen(line);

  while(file >> line) {
    strcpy(arr[index], line);
    ++index;
  }

  printf("simple treecount:   \t%d\n", count_trees(arr, len, index, 3, 1));  
  printf("multiple treecount: \t%d\n", count_multiple_trees(arr, len, index));  


  file.close();
  return 0;
}
