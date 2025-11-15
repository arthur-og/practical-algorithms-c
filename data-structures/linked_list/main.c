#include "linked_list.h" // Inclui nossa nova biblioteca
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
  list_append_reference(list, list2, list_get_total_bytes(list2), list_print);

  list_append(list, str2, sizeof(str2), print_string);

  list_print(list, 0);
  putchar('\n');
  // list_inverter(list);
  //  printf("inverted:\n");
  //  list_print(list, 2);
  free_list(list);
  free_list(list2);
}
