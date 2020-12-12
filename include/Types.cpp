#pragma once

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

  Node<T>* get_tail(Node<T>* targetHead) {
    while(targetHead && targetHead->next) {
      targetHead = targetHead->next;
    }
    return targetHead;
  }

  void insert_end(T data) {
    Node<T>* node = (Node<T>*)malloc(sizeof(Node<T>));
    node->data = data;
    node->next = nullptr;

    Node<T>* tail = get_tail(head);
    tail->next = node;
  }

  void print() {
    Node<T>* current = head;
    while(current) {
      printf("\t%d\n", current->data);
      current = current->next;
    }
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


