#include <stdio.h>
#include <fstream>
#include <cstdint>
#include <string>
#include <regex>

#include "Timer.cpp"

typedef uint16_t uint16;
typedef uint8_t uint8;
typedef int32_t int32;

inline bool parse_attribute(char* attribute, char* value) {
  if(!strcmp(attribute, "byr")) {
    int32 v = atoi(value);
    return v >= 1920 && v <= 2002;
  } 
  else if(!strcmp(attribute, "iyr")) {
    int32 v = atoi(value);
    return v >= 2010 && v <= 2020;
  }
  else if(!strcmp(attribute, "eyr")) {
    int32 v = atoi(value);
    return v >= 2020 && v <= 2030;
  }
  else if(!strcmp(attribute, "hgt")) {
    std::smatch match;
    std::string str(value);
    std::regex_match(str, match, std::regex("^([0-9]*)(cm|in)$"));
    if(match.empty()) return false;
    int32 v = atoi(match.str(1).c_str());
    if(match[2] == "in") return v >= 59  && v <= 76;
    return v >= 150 && v <= 193; 
  }
  else if(!strcmp(attribute, "hcl")) {
    std::string str(value);
    return std::regex_match(str, std::regex("#[0-9a-f]{6}")); 
  }
  else if(!strcmp(attribute, "ecl")) {
    std::string str(value);
    return std::string("amb blu brn gry grn hzl oth").find(str) != std::string::npos; 
  }
  else if(!strcmp(attribute, "pid")) {
    std::string str(value);
    return std::regex_match(str, std::regex("[0-9]{9}")); 
  }
  return true;
}

inline void count_valid_and_present(char** attributes, int32 amt, char* line, int32& count) {
  char* token = strtok(line, ": ");
  while(token) {
    char cpy[256];
    strcpy(cpy, token);

    bool valid = false;
    for(uint8 j = 0; j < amt; ++j) {
      valid |= !strcmp(attributes[j], token);
    }
    token = strtok(0, ": ");
    count += valid && parse_attribute(cpy, token);
    token = strtok(0, ": ");
  }
}

inline void count_valid(char** attributes, int32 amt, char* line, int32 lineLen, int32& count) {
  for(int32 i = 0; i < lineLen; ++i) {
    if(line[i] != ':') continue;
    
    for(uint8 j = 0; j < amt; ++j) {
      count += attributes[j][0] == line[i - 3] &&
               attributes[j][1] == line[i - 2] &&
               attributes[j][2] == line[i - 1];
    }
  }
}

int main() {
  TIMER("Total solution time");

  FILE* file = fopen("res/4/data.txt", "r");
  if(!file) {
    printf("file wasn't found\n");
    return 1;
  }

  const int32 amt = 7;
  const char* attributes[amt] = {
    "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"
  };
  
  int32 result1 = 0;
  int32 result2 = 0;
  int32 count1 = 0;
  int32 count2 = 0;
  int32 ln = 0;

  char* line = (char*)malloc(512);
  while(true) {
    char* r = fgets(line, 512, file);
    int32 len = strlen(line) - 1;
    line[len] = 0;

    if(!len || !r) {
      result1 += count1 == amt;
      result2 += count2 == amt;
      count1 = 0;
      count2 = 0;
    }

    if(!r) break;

    count_valid((char**)attributes, amt, line, len, count1);
    count_valid_and_present((char**)attributes, amt, line, count2);
  }

  printf("result: %d\n", result1);
  printf("result: %d\n", result2); 

  free(line);
  fclose(file);
  return 0;
}
