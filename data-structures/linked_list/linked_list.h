#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*PrintFunc)(void *data, int len);

typedef struct node Node;

typedef struct linkedlist LinkedList;

Node *create_node(void *dt, int len, PrintFunc print_func);

static Node *create_reference_node(void *dt, size_t len, PrintFunc print_func);
LinkedList *create_list();
int list_append(LinkedList *list, void *dt, int len, PrintFunc print_func);

int list_append_reference(LinkedList *list, void *dt, int len,
                          PrintFunc print_func);
void list_print(void *lista, int len);

void list_inverter(LinkedList *list);
void free_list(LinkedList *list);

void free_list_shallow(LinkedList *list);
int list_get_node_count(LinkedList *list);

int list_get_total_bytes(LinkedList *list);

#endif // LINKEDLIST_H
