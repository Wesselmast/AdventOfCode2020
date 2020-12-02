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

  void print() {
    Node<T>* current = head;
    while(current) {
      printf("\t%d\n", current->data);
      current = current->next;
    }
  }

  Node<T>* get_tail(Node<T>* targetHead) {
    while(targetHead && targetHead->next) {
      targetHead = targetHead->next;
    }
    return targetHead;
  }

  Node<T>* quick_sort_internal(Node<T>* begin, Node<T>* tail) {
    if(!begin || begin == tail) {
      return begin;
    }

    Node<T>* current = begin; 
    Node<T>* pivot = tail;
    Node<T>* previous = nullptr;
    Node<T>* newHead = nullptr;
    Node<T>* newTail = nullptr;

    while(current != pivot) {
      if(current->data < pivot->data) {
	if(!newHead) newHead = current;
	previous = current;
	current = current->next;
      }
      else {
	if(previous) previous->next = current->next;
	Node<T>* tmp = current->next;
	current->next = nullptr;
	tail->next = current;
	tail = current;
	current = tmp;
      }
    }

    if(!newHead) newHead = pivot;
    newTail = tail;

    if(newHead != pivot) {
      Node<T>* tmp = newHead;
      while(tmp->next != pivot) {
	tmp = tmp->next;
      }
      tmp->next = nullptr;

      newHead = quick_sort_internal(newHead, tmp);

      tmp = get_tail(newHead);
      tmp->next = pivot;
    }

    pivot->next = quick_sort_internal(pivot->next, newTail);

    return newHead;
  }

  void quick_sort() {
    head = quick_sort_internal(head, get_tail(head));
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
  return 0;
}
