#include <stdio.h>

#include "Timer.cpp"
#include "Types.cpp"

#define R2D 3.14159265 / 180

inline void inc_degrees(int32& degrees, int32 by) {
  degrees = (degrees + by + 360) % 360;
}

inline void rotate_waypoint(int32& forwardAxis, int32& rightAxis, int32 value) {
  int32 temp = (forwardAxis * int32(cos(value * R2D))) - (rightAxis   * int32(sin(value * R2D)));
  rightAxis  = (rightAxis   * int32(cos(value * R2D))) + (forwardAxis * int32(sin(value * R2D)));
  forwardAxis = temp;
}

inline void go_forward(int32& forwardAxis, int32& rightAxis, int32 degrees, int32 value) {
  rightAxis   += int32(sin(degrees * R2D) * value);
  forwardAxis += int32(cos(degrees * R2D) * value);
}

int main() {
  TIMER("Solution total time");
  
  FILE* file = fopen("res/12/data.txt", "r");
  if(!file) {
    printf("file wasn't found\n");
    return 1;
  }
 
  int32 shipX1 = 0;
  int32 shipY1 = 0;
  int32 degrees = 90;

  int32 waypointX = 1;
  int32 waypointY = 10;
  int32 shipX2 = 0;
  int32 shipY2 = 0;

  while(true) {
    char cmd;
    int32 value;
    int32 ln = fscanf(file, "%c%d\n", &cmd, &value);
    if(ln == EOF) break;

    switch(cmd) {
      case 'N': {
	shipX1 += value;
	waypointX += value;
	break;
      }
      case 'S': {
	shipX1 -= value;
	waypointX -= value;
	break;
      }
      case 'E': {
	shipY1 += value;
	waypointY += value;
	break;
      }
      case 'W': {
	shipY1 -= value;         
	waypointY -= value;
	break;
      }
      case 'L': {
	inc_degrees(degrees, -value);
	rotate_waypoint(waypointX, waypointY, -value);
	break;
      }
      case 'R': {
	inc_degrees(degrees,  value); 
	rotate_waypoint(waypointX, waypointY, value);
	break;
      }
      case 'F': {
	go_forward(shipX1, shipY1, degrees, value);
	shipX2 += waypointX * value;
	shipY2 += waypointY * value;
	break;
      }
    }
  }

  printf("PUZZLE 1: %d\n", abs(shipX1) + abs(shipY1));
  printf("PUZZLE 2: %d\n", abs(shipX2) + abs(shipY2));

  fclose(file);
  return 0;
}
