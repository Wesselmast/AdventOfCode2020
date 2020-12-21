#include <stdio.h>

#include "Timer.cpp"
#include "Types.cpp"

uint64 inverse_modulo(uint64 n, uint64 m) {
  n = n % m;
  for(uint64 i = 1; i < m; ++i) {
    if((n * i) % m == 1) return i;
  }
  printf("couldn't find the inverse modulo of {%llu, %llu}\n", n, m);
  return 0;
}

int main() {
  TIMER("Solution total time");
  
  FILE* file = fopen("res/13/data.txt", "r");
  if(!file) {
    printf("file wasn't found\n");
    return 1;
  }

  int32 start;
  char line[256];
  int32 arr[256];
  int32 index = 0;

  fscanf(file, "%d\n", &start);
  fscanf(file, "%s\n", line);
  char* id = strtok(line, ",");
  while(id) {
    arr[index] = atoi(id);
    id = strtok(0, ",");
    ++index;
  }

  int32 smallest = INT32_MAX;
  int32 smallestID;
  uint64 product = 1;

  for(int32 i = 0; i < index; ++i) {
    int32 id = arr[i];
    if(!id) continue;
    product *= id;
    int32 wait = id - (start % id);
    if(wait < smallest) {
      smallestID = id;
      smallest = wait;
    }
  }

  uint64 t = 0;
  for(int32 i = 0; i < index; ++i) {
    int32 id = arr[i];
    if(!id) continue;
    uint64 pp = product / id;
    t += (id - i) * inverse_modulo(pp, id) * pp;
  }

  printf("ID times the smallest wait time:     \t%d\n", smallestID * smallest);
  printf("Earliest timestamp all busses align: \t%llu\n", t % product);
  return 0;
} 
