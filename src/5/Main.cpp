#include <stdio.h>
#include <fstream>
#include <cstdint>
#include <math.h>
#include <vector>
#include <algorithm>

#include "Timer.cpp"
#include "Types.cpp"

inline uint8 string_median(char* input, char right, char left, 
			   uint8 first, uint8 last, uint8 low, uint8 high) {
  char lastC;
  for(int32 i = first; i < last; ++i) {
    uint8 num = floor((high + low) * 0.5f);   
    if(input[i] == right) low = num;
    else if(input[i] == left) high = num;
    lastC = input[i];
  }
  return lastC == right ? low + 1 : high;
}

inline int32 calc_seat_ID(char* input, uint8& row, uint8& col) {
  row = string_median(input, 'B', 'F', 0,  7, 0, 127); 
  col = string_median(input, 'R', 'L', 7, 10, 0, 7); 
  return int32(row * 8 + col); 
}

inline int32 find_my_seat(const std::vector<int32>& seats) {
  int32 lastSeat = seats[0] - 1;
  for(int32 i = 0; i < seats.size(); ++i) {
    if(seats[i] - 1 != lastSeat) return seats[i] - 1;
    lastSeat = seats[i];
  }
  return -1;
}

int main() {
  TIMER("Solution total time");
  
  std::ifstream file("res/5/data.txt", std::ios::in);
  if(!file.good()) {
    printf("file wasn't found\n");
    return 1;
  }

  int32 highest = 0;
  char line[10];
  std::vector<int32> seats;
  
  while(file >> line) {
    uint8 row, col;
    int32 seatID = calc_seat_ID(line, row, col);
    if(seatID > highest) highest = seatID;
    if(row != 0 && row != 127) seats.push_back(seatID);
  }
  std::sort(seats.begin(), seats.end());
  
  printf("highest seat ID: %d\n", highest);
  printf("my seat ID:      %d\n", find_my_seat(seats));
  return 0;
}
