#include <stdio.h>
#include <cstdint>
#include <vector>

#include "Timer.cpp"

typedef int32_t int32;
typedef uint8_t uint8;

struct Command { 
  char cmd;
  int32 val;
  int32 done = 0;
};

inline bool execute(std::vector<Command>& commands, int32& pointer, int32& accumulator) {
  if(pointer >= commands.size()) return true;
  
  Command& cmd = commands[pointer];
  if(cmd.done) return false;

  switch(cmd.cmd) {
    case 'a': accumulator += cmd.val; break;
    case 'j': pointer += cmd.val - 1; break;
  }

  pointer++;
  cmd.done = 1;
  return execute(commands, pointer, accumulator);
}

int32 try_fixing(std::vector<Command>& commands) {
  for(int32 i = 0; i < commands.size(); ++i) {
    std::vector<Command> newCommands(commands);
    Command& cmd = newCommands[i];
    
    bool changed = true;
    switch(cmd.cmd) {
      case 'j': cmd.cmd = 'n';  break;
      case 'n': cmd.cmd = 'j';  break;
      default: changed = false; break;   
    };
    if(!changed) continue;
  
    int32 accumulator = 0;
    int32 pointer = 0;
    bool res = execute(newCommands, pointer, accumulator);
    
    if(res) return accumulator;
  }
  return -1;
}

int main() {
  TIMER("Solution total time");
  
  FILE* file = fopen("res/8/data.txt", "r");
  if(!file) {
    printf("file wasn't found\n");
    return 1;
  }

  std::vector<Command> commands;

  while(true) {
    Command cmd;
    char pm;

    int32 ln = fscanf(file, "%3c %c%d\n", &cmd.cmd, &pm, &cmd.val);
    cmd.val *= cmd.cmd != 'n';
    cmd.val *= pm == '-' ? -1 : 1;

    if(ln == EOF) break;
    commands.push_back(cmd);
  }

  std::vector<Command> puzzle1Commands(commands);
  std::vector<Command> puzzle2Commands(commands);
  
  int32 accumulator = 0;
  int32 pointer = 0;
  execute(puzzle1Commands, pointer, accumulator); 
  
  printf("accumulator before fix:  %d\n", accumulator);
  printf("accumulator after fix:   %d\n", try_fixing(puzzle2Commands));

  return 0;
}
