#include <stdio.h>
#include <cstdint>
#include <vector>
#include <string>

#include "Timer.cpp"

typedef int32_t int32;
typedef uint8_t uint8;

struct Bag {
  std::string stringID;
  int32 value;
  std::vector<Bag> bags;
};


inline bool contains(std::vector<std::string>& v, const std::string& s) {
  return std::find(v.begin(), v.end(), s) != v.end();
}

inline bool contains(Bag& parent, Bag& child) {
  for(Bag& b : parent.bags) {
    if(b.stringID == child.stringID) return true;
  }
  return false;
} 

inline void count_bags(std::vector<Bag>& bags, std::vector<std::string>& valid, Bag& in, int32& res) {
  if(contains(valid, in.stringID)) return;
  ++res;
  valid.push_back(in.stringID);
  for(Bag& bag : bags) {
    if(contains(valid, bag.stringID)) continue;
    if(contains(bag, in)) {
      count_bags(bags, valid, bag, res);
    }
  }
}

inline void count_bags_two(std::vector<Bag>& bags, std::string bagID, std::vector<int32>& arr) {
  for(Bag& bag : bags) {
    if(bag.stringID == bagID) {
      for(int32 i = 0; i < bag.bags.size(); ++i) {
	arr[i] *= bag.bags[i].value;
	arr[i] += bag.bags[i].value;
	count_bags_two(bags, bag.bags[i].stringID, arr);
      }
      return;
    }
  }
}

int main() {
  TIMER("Solution total time");
  
  FILE* file = fopen("res/7/data.txt", "r");
  if(!file) {
    printf("file wasn't found\n");
    return 1;
  }
  
  int32 res = 0;
  int32 res2 = 0;
  std::vector<std::string> valid;
  std::vector<std::string> valid2;
  std::vector<Bag> bags;
  valid.push_back("shinygold");
  valid2.push_back("shinygold");

  char* line = (char*)malloc(512);
  while(true) {
    char* r = fgets(line, 512, file);
    int32 len = strlen(line) - 1;
    line[len] = 0;

    if(!r) break;
    
    Bag bag;
    char* token = strtok(line, " ");
    bag.stringID = token;
    token = strtok(0, " ");
    bag.stringID.append(token);
    token = strtok(0, " ");
    token = strtok(0, " ");
    token = strtok(0, " ");
   
    while(token) {
      Bag inBag;
      inBag.value = atoi(token);
      token = strtok(0, " ");
      inBag.stringID = token;
      token = strtok(0, " ");
      inBag.stringID.append(token);
      token = strtok(0, " ");
      token = strtok(0, " ");
     
      if(!inBag.value) continue;
      if(contains(valid, inBag.stringID)) {
	count_bags(bags, valid, bag, res);
      }
      bag.bags.push_back(inBag);
    }
    bags.push_back(bag);
  }
  
  std::vector<int32> arr;
  arr.resize(5);

  count_bags_two(bags, "shinygold", arr);
  for(int32 i = 0; i < arr.size(); ++i) {
    printf("%d\n", arr[i]);
    res2 += arr[i];
  }
  printf("%d\n", res);
  printf("%d\n", res2);
 


  free(line);
  fclose(file);
}
