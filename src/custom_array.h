#ifndef CUSTOMARRAY_H
#define CUSTOMARRAY_H
typedef struct Index {
  int i;
  int j;
} Index;

typedef struct indices_array {
  Index *array_core;
  unsigned int size;
  unsigned int capacity;
} indices_array;

// I will be using the C style of returning -1
// when error thats why these
// are int returning functions
int assert(indices_array *list);
int add_element(indices_array *list, Index element);
int remove_element(indices_array *list, unsigned int position);
int resize_array(indices_array *list);
Index get_element_in(indices_array *list, unsigned int position);

#endif // !CUSTOMARRAY_H
