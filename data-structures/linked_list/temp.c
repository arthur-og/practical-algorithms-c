void cool_print(LinkedList *list) {
  Node *node = list->head;
  for (int i = 1; i <= list->size - 1; ++i) {

    printf("       %d       -->", *(int *)node->data);
    node = node->Next;
  }
  printf("       %d\n", *(int *)list->tail->data);

  node = list->head;
  for (int i = 1; i <= list->size - 1; ++i) {

    printf(" %p -->", node->Next);
    node = node->Next;
  }
  printf("      %p\n", list->tail->Next);
}
