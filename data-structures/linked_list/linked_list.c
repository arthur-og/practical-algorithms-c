#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_int(void *data, int len) {
  printf("       %d       ", *(int *)data);
}
void print_float(void *data, int len) { printf("     %f    ", *(float *)data); }
void print_string(void *data, int len) { printf("    %s   ", (char *)data); }

void print_int_arr(void *data, int len) {

  int count = len / sizeof(int);
  printf("    [");
  for (int i = 0; i < count; ++i) {
    printf(" %d ", ((int *)data)[i]);
  }
  printf("]     ");
}

typedef void (*PrintFunc)(void *data, int len);

typedef struct Node {
  void *data;
  int data_len;
  struct Node *next;
  PrintFunc print_pointer;
} Node;

typedef struct {
  Node *head;
  Node *tail;
  int size;
} LinkedList;

Node *create_node(void *dt, int len, PrintFunc print_func) {

  Node *node = (Node *)malloc(sizeof(Node));
  if (node == NULL) {
    printf("node aloccation fail");
    return NULL;
  }

  node->data = malloc(len);
  if (node->data == NULL) {
    printf("data node aloccation fail");
    free(node);
    return NULL;
  }

  memcpy(node->data, dt, len);
  node->data_len = len;
  node->next = NULL;
  node->print_pointer = print_func;
  return node;
}

LinkedList *create_list() {
  LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
  if (list == NULL) {
    printf("list aloccation fail");
    return NULL;
  }
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;
  return list;
}

void list_append(LinkedList *list, void *dt, int len, PrintFunc print_func) {
  Node *node = create_node(dt, len, print_func);
  if (node == NULL)
    return;

  if (list->size == 0) {
    list->head = node;
    list->tail = node;
  } else {
    list->tail->next = node;
    list->tail = node;
  }
  list->size++;
}

void list_print(void *lista, int len) {
  LinkedList *list = (LinkedList *)lista;
  if (list->head == NULL) {
    printf("List is empty");
    return;
  }
  putchar('|');
  Node *current = list->head;
  while (current != NULL) {
    current->print_pointer(current->data, current->data_len);
    if (current->next != NULL) {
      printf("  -->");
    }

    current = current->next;
  }
  // putchar('\n');
  /*
    current = list->head;
    while (current != NULL) {
      printf("  %p  ", (void *)current->next);
      if (current->next != NULL) {
        printf(" -->");
      }
      current = current->next;
    }
    */
  putchar(' ');
  putchar('|');
  // putchar('\n');
};

void list_inverter(LinkedList *list) {
  Node *prev = list->head;
  Node *current = list->head->next;
  list->head->next = NULL;
  Node *next_node;

  while (current != NULL) {
    next_node = current->next;
    current->next = prev;
    prev = current;
    current = next_node;
  }
  Node *temp = list->head;
  list->head = list->tail;
  list->tail = temp;
}

void free_list(LinkedList *list) {
  if (list == NULL)
    return;

  Node *current = list->head;
  Node *next_node;

  while (current != NULL) {
    next_node = current->next;
    free(current->data);
    free(current);
    current = next_node;
  }
  free(list);
}

int list_len(LinkedList *list) {
  if (list == NULL)
    return -1;

  Node *current = list->head;
  Node *next_node;
  int len = 0;

  while (current != NULL) {
    next_node = current->next;
    len += current->data_len;
    current = next_node;
  }

  return len;
}

int main() {
  int value1 = 12;
  float value2 = 12.123;
  char str1[] = "star wars";
  float value4 = 129.78;
  int arr[] = {1, 12, 5, 6, 9};
  char str2[] = "horcrux";

  LinkedList *list = create_list();

  list_append(list, &value1, sizeof(value1), print_int);
  list_append(list, &value2, sizeof(value2), print_float);

  LinkedList *list2 = create_list();

  list_append(list2, &str1, sizeof(str1), print_string);
  list_append(list2, &value4, sizeof(value4), print_float);
  list_append(list2, arr, sizeof(arr), print_int_arr);
  list_append(list, list2, list_len(list2), list_print);

  list_append(list, str2, sizeof(str2), print_string);

  list_print(list, 0);
  putchar('\n');
  // list_inverter(list);
  //  printf("inverted:\n");
  //  list_print(list, 2);
}
