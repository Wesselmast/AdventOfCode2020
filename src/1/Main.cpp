#include <stdio.h>
#include <fstream>
#include <cstdint>
#include <string>

#include "Timer.cpp"
#include "Types.cpp"
#include "List.cpp"

inline int32 fetch_expense_report(const List<int32>& list) {
  Node<int32>* x = list.head;
  while(x) {
    Node<int32>* y = list.head;
    while(y) {
      if(x->data + y->data > 2020) break;
      if(x->data + y->data == 2020) {
	return x->data * y->data;
      }
      y = y->next;
    }
    x = x->next;
  }
  printf("couldn't crunch the numbers!\n");
  return -1;
}

inline int32 fetch_expense_report_deeper(const List<int32>& list) {
  Node<int32>* x = list.head;
  while(x) {
    Node<int32>* y = list.head;
    while(y) {
      Node<int32>* z = list.head;
      while(z) {
	if(x->data + y->data + z->data > 2020) break;
	if(x->data + y->data + z->data == 2020) {
	  return x->data * y->data * z->data;
	}
	z = z->next;
      }
      y = y->next;
    }
    x = x->next;
  }
  printf("couldn't crunch the numbers! (deeper)\n");
  return -1;
}

int main() {
  TIMER("Solution total time");
  
  std::ifstream file("res/1/data.txt", std::ios::in);
  if(!file.good()) {
    printf("file wasn't found\n");
    return 1;
  }

  List<int32> list;
  int32 line;
  while(file >> line) list.insert(line); 

  list.quick_sort();

  printf("expense report 1: %d\n", fetch_expense_report(list));
  printf("expense report 2: %d\n", fetch_expense_report_deeper(list));
  
  list.free_list();
  file.close();
  return 0;
}
