#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_int(void *data) { printf("        %d      ", *(int *)data); }
void print_float(void *data) { printf("    %f   ", *(float *)data); }
void print_string(void *data) { printf("    %s    ", (char *)data); }

typedef struct Node {
  void *data;
  struct Node *Next;
  void (*print_pointer)(void *data);
} Node;

typedef struct {
  Node *head;
  Node *tail;
  int size;
} LinkedList;

LinkedList *new(void *dt, int len, void (*print_func)(void *data)) {
  LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
  Node *no = (Node *)malloc(sizeof(Node));
  no->data = malloc(len);
  no->Next = NULL;
  no->print_pointer = print_func;
  memcpy(no->data, dt, len);

  list->head = no;
  list->tail = no;
  list->size = 1;
  return list;
}

void add_tail(LinkedList *list, void *dt, int len,
              void (*print_func)(void *data)) {
  Node *no = (Node *)malloc(sizeof(Node));
  no->data = malloc(len);
  memcpy(no->data, dt, len);
  list->tail->Next = no;
  list->tail = no;
  no->Next = NULL;

  no->print_pointer = print_func;

  list->size++;
}

void generic_print(LinkedList *list) {
  Node *no = list->head;
  for (int i = 1; i <= list->size - 1; ++i) {
    no->print_pointer(no->data);
    printf("-->");
    no = no->Next;
  }
  no->print_pointer(no->data);
  putchar('\n');

  no = list->head;
  for (int i = 1; i <= list->size - 1; ++i) {
    printf(" %p -->", no->Next);
    no = no->Next;
  }
  printf("       %p    ", no->Next);

  putchar('\n');
};

int main() {
  int valor1 = 12;
  float valor2 = 12.123;
  char valor3[] = "ai dento";
  float valor4 = 129.78;

  LinkedList *list = new (&valor1, sizeof(valor1), print_int);

  add_tail(list, &valor2, sizeof(valor2), print_float);
  add_tail(list, &valor3, sizeof(valor3), print_string);
  add_tail(list, &valor4, sizeof(valor4), print_float);
  generic_print(list);
}
