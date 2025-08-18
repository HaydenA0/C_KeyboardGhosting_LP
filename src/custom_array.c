#include "custom_array.h"
#include <stdio.h>
#include <stdlib.h>

int assert(indices_array *list) {
  if (list == NULL) {
    printf("List not valid, null pointe\n");
    return -1;
  }
  if (list->size > list->capacity) {
    printf("List not valid, size > capacity\n");
    return -1;
  }
  if (list->capacity == 0 && list->array_core != NULL) {
    printf("List not valid, has no capacity but has memory\n");
    return -1;
  }
  if (list->size != 0 &&
      list->array_core == NULL) { // I dont think it's capacity but rather size
    printf("List not valid, has capacity but has no memory\n");
    return -1;
  }
  return 0;
};
int add_element(indices_array *list, Index element) {
  int fail = 0;
  if (list->size == list->capacity) {
    fail = resize_array(list);
  }
  if (fail == 0) {
    list->array_core[list->size] = element;
    list->size++;
    return 0;
  } else {
    return fail;
  }
};
int remove_element(indices_array *list, unsigned int position) {
  // I will not be using realloc here that is left for resize_array
  // which I am not that familiar with
  // But I think I should idk
  // my idea is to copy everything after list[position] and
  // and paste it on top of list[position]
  // last elemnt would be given the value of (0,0)
  // and size decreases
  if (position >= list->size) {
    printf("Error : Trying to delete a position that doesnt exist");
    return -1;
  }
  for (int i = position; i < list->size - 1; i++) {
    list->array_core[i] = list->array_core[i + 1];
  }
  list->size--;
  return 0;
};
int resize_array(indices_array *list) {
  if (list->capacity == 0) {
    list->capacity = 5;
  }

  list->capacity = list->capacity * 2;
  int i_capacity = list->capacity;
  Index *temp_ptr =
      (Index *)realloc(list->array_core, sizeof(Index) * i_capacity);
  // in case realloc doesnt free and gives a NULL ptr

  if (temp_ptr == NULL) {
    printf("Failled reallocation\n");
    return -1;
  }
  list->array_core = temp_ptr;
  return 0;
};
Index get_element_in(indices_array *list, unsigned int position) {
  Index error_index = {-1, -1}; // sentinel for error

  if (list == NULL || list->array_core == NULL) {
    return error_index;
  }
  if (position >= list->size) {
    return error_index;
  }

  return list->array_core[position];
}
