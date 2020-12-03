#include <stdio.h>
#include <fstream>
#include <cstdint>

#include "Timer.cpp"

typedef uint16_t uint16;
typedef int32_t int32;

inline void validate_password_old(int32& result, char* str, char c, int32 low, int32 high) {
  int32 count = 0;
  for(int32 i = 0; str[i]; ++i) {
    count += (str[i] == c);
  }
  result += (count >= low && count <= high);
}

inline void validate_password_new(int32& result, char* str, char c, int32 first, int32 second) {
  result += (str[first - 1] == c ^ str[second - 1] == c); 
}

int main() {
  TIMER("Total solution time");

  FILE* file = fopen("res/2/data.txt", "r");
  if(!file) {
    printf("file wasn't found\n");
    return 1;
  }

  int32 result1 = 0;
  int32 result2 = 0;
  int32 ln = 0;

  while(true) {
    int32 low, high;
    char c;
    char str[256];
   
    ln = fscanf(file, "%d-%d %c: %256s", &low, &high, &c, str);  
    if(ln == EOF) break;

    validate_password_old(result1, str, c, low, high);
    validate_password_new(result2, str, c, low, high);
 }
  
  printf("puzzle 1 solution: %d\n", result1);
  printf("puzzle 2 solution: %d\n", result2);

  fclose(file);
  return 0;
}
