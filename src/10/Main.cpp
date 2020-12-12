#include <stdio.h>
#include <cstdint>
#include <fstream>
#include <math.h>

#include "Timer.cpp"
#include "Types.cpp"

typedef int32_t int32;
typedef uint64_t uint64;

void calculate_jolt_differences(int32& one, int32& three, const List<int32>& adapters) {
  Node<int32>* current = adapters.head;
  while(current->next) {
    int32 d = current->next->data - current->data;
    one   += d == 1;
    three += d == 3;
    current = current->next;
  }
}

void calculate_adapter_combinations(uint64& combinations, const List<int32>& adapters) {
  Node<int32>* current = adapters.head;
  int32 temp = 0;

  while(current->next) {
    int32 d = current->next->data - current->data;
    if(d != 3) ++temp;
    else if(temp > 0) {
      combinations *= pow(2, temp - 1) - (temp == 4);
      temp = 0;
    }
    current = current->next;
  }
}


int main() {
  TIMER("Solution total time");
  
  std::ifstream file("res/10/data.txt", std::ios::in);
  if(!file.good()) {
    printf("file wasn't found\n");
    return 1;
  }

  List<int32> adapters;
  adapters.insert(0);

  int32 line;
  while(file >> line) adapters.insert(line); 

  adapters.quick_sort();
  adapters.insert_end(adapters.get_tail(adapters.head)->data + 3);

  int32 one = 0, three = 0;
  calculate_jolt_differences(one, three, adapters);

  printf("mul of jolt differences: %d\n", one * three);

  uint64 combinations = 1;
  calculate_adapter_combinations(combinations, adapters);

  printf("combinations: %llu\n", combinations);

  adapters.free_list();
  return 0;
}
