#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
  void *data;
  int data_reference;
  int data_len;
  struct node *next;
  PrintFunc print_pointer;
};

struct linkedlist {
  Node *head;
  Node *tail;
  int size;
};

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
  node->data_reference = 0;
  node->next = NULL;
  node->print_pointer = print_func;
  return node;
}

static Node *create_reference_node(void *dt, size_t len, PrintFunc print_func) {
  Node *node = malloc(sizeof(Node));
  if (node == NULL) {
    printf("node aloccation fail");
    return NULL;
  }
  node->data = dt;
  node->data_len = len;
  node->next = NULL;
  node->print_pointer = print_func;
  node->data_reference = 1;
  return node;
}
static void append(LinkedList *list, Node *node) {
  if (list->size == 0) {
    list->head = node;
    list->tail = node;
  } else {
    list->tail->next = node;
    list->tail = node;
  }
  list->size++;
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

int list_append(LinkedList *list, void *dt, int len, PrintFunc print_func) {
  Node *node = create_node(dt, len, print_func);
  if (node == NULL)
    return -1;
  append(list, node);
  return 0;
}

int list_append_reference(LinkedList *list, void *dt, int len,
                          PrintFunc print_func) {
  Node *node = create_reference_node(dt, len, print_func);
  if (node == NULL)
    return 91;
  append(list, node);
  return 0;
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
    if (current->data_reference == 0) {
      // Só libera 'data' se NÃO for uma referência
      free(current->data);
    }

    free(current);
    current = next_node;
  }
  free(list);
}

void free_list_shallow(LinkedList *list) {
  if (list == NULL)
    return;

  Node *current = list->head;
  Node *next_node;

  while (current != NULL) {
    next_node = current->next;
    // not free the data

    free(current);
    current = next_node;
  }
  free(list);
}

int list_get_node_count(LinkedList *list) {
  if (list == NULL)
    return -1;
  return list->size;
}

int list_get_total_bytes(LinkedList *list) {
  if (list == NULL)
    return -1;

  Node *current = list->head;
  int total = 0;

  while (current != NULL) {
    total += current->data_len;
    current = current->next;
  }

  return total;
}
