#include <stdio.h>
#include <fstream>
#include <cstdint>
#include <string>

#include "Timer.cpp"

typedef uint16_t uint16;
typedef int32_t int32;

template<typename T>
struct Node {
  Node<T>* next = nullptr;
  T data;
};

template<typename T>
struct List {
  Node<T>* head = nullptr;
  
  void insert(T data) {
    Node<T>* node = (Node<T>*)malloc(sizeof(Node<T>));
    node->data = data;
    node->next = head;
    head = node;
  }
  
  void free_list() {
    Node<T>* current = head;
    Node<T>* next = nullptr;
    while(current) {
      next = current->next;
      free(current);
      current = next;
    }
    head = nullptr;
  }

  ~List() {
    free_list();
  }
};

inline int32 fetch_expense_report(const List<int32>& list) {
  Node<int32>* x = list.head;
  while(x) {
    Node<int32>* y = list.head;
    while(y) {
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
  }

  List<int32> list;
  int32 line;
  while(file >> line) list.insert(line); 

  printf("expense report 1: %d\n", fetch_expense_report(list));
  printf("expense report 2: %d\n", fetch_expense_report_deeper(list));
  return 0;
}
