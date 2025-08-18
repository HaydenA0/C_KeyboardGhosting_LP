#include "matrice.h"
#include "custom_array.h"
#include <stdio.h>
#include <stdlib.h>

int assert_matrice(Matrice *matrice) {
  if (matrice->rows * matrice->lines != 0 && matrice->array_core == NULL) {
    printf("Matrice is not valid [Has dimensions but no memory]\n");
    return -1;
  }
  if (matrice->rows * matrice->lines == 0 && matrice->array_core != NULL) {
    printf("Matrice is not valid [Is empty but has a memory pointer]\n");
    return -1;
  }
  return 0;
};

Matrice m_difference(Matrice *matrice1, Matrice *matrice2) {
  // matrice with 1x1 dimensions and no memory is not valid !
  Matrice result;
  if (matrice1->lines != matrice2->lines || matrice1->rows != matrice2->rows) {
    printf("Error Not the same size \n");
    result.lines = 1;
    result.rows = 1;
    result.array_core = NULL;
    return result;
  }

  int row = matrice1->rows;
  result.array_core =
      (int *)malloc(sizeof(int) * matrice1->rows * matrice1->lines);
  if (result.array_core == NULL) {
    printf("Error allocating data");
    result.lines = 1;
    result.rows = 1;
    return result;
  }
  result.lines = matrice2->lines;
  result.rows = row;
  for (int i = 0; i < result.lines; i++) {
    for (int j = 0; j < result.rows; j++) {
      *(result.array_core + i * row + j) =
          *(matrice1->array_core + i * row + j) -
          *(matrice2->array_core + i * row + j);
    }
  }
  return result;
};

int assert_c_matrice(C_Matrice *matrice) {
  if (matrice->rows * matrice->lines != 0 && matrice->array_core == NULL) {
    printf("Matrice is not valid [Has dimensions but no memory]\n");
    return -1;
  }
  if (matrice->rows * matrice->lines == 0 && matrice->array_core != NULL) {
    printf("Matrice is not valid [Is empty but has a memory pointer]\n");
    return -1;
  }
  return 0;
};

int i_get_element(Matrice *matrice, int i, int j) {
  if (matrice == NULL || matrice->array_core == NULL)
    return -1;

  if (i < 0 || j < 0 || (unsigned int)i >= matrice->lines ||
      (unsigned int)j >= matrice->rows) {
    return -1;
  }

  unsigned int index = i * matrice->rows + j;
  return matrice->array_core[index];
}

char c_get_element(C_Matrice *matrice, int i, int j) {
  if (matrice == NULL || matrice->array_core == NULL)
    return '\0';

  if (i < 0 || j < 0 || (unsigned int)i >= matrice->lines ||
      (unsigned int)j >= matrice->rows) {
    return '\0';
  }

  unsigned int index = i * matrice->rows + j;
  return matrice->array_core[index];
}
char *find_masked_chars(Matrice *circuit, C_Matrice *table) {
  indices_array index_list;
  index_list.capacity = 20; // big room
  index_list.size = 0;
  index_list.array_core = (Index *)malloc(index_list.capacity * sizeof(Index));
  int size = 0;

  for (int i = 0; i < circuit->lines; i++) {
    for (int j = 0; j < circuit->rows; j++) {
      if (i_get_element(circuit, i, j) == 1) {
        size++;
        Index index;
        index.i = i;
        index.j = j;
        add_element(&index_list, index);
      }
    }
  }
  char *keys_pressed = calloc(size + 1, sizeof(char));
  if (keys_pressed == NULL) {
    printf("Error allocating memory for keys_pressed \n");
    return NULL;
  }
  for (int i = 0; i < index_list.size; i++) {
    Index index = index_list.array_core[i];
    int line = index.i;
    int row = index.j;
    char key = c_get_element(table, line, row);
    keys_pressed[i] = key;
  }
  free(index_list.array_core);
  return keys_pressed;

}; // Kept the same Matrice name

void copy_to(Matrice *matrice_to_copy, Matrice *matrice) {
  matrice->rows = matrice_to_copy->rows;
  matrice->lines = matrice_to_copy->lines;
  matrice->array_core =
      (int *)malloc(sizeof(int) * matrice->lines * matrice->rows);
  // todo : addd checking
  for (int i = 0; i < matrice->lines; i++) {
    for (int j = 0; j < matrice->rows; j++) {
      *(matrice->array_core + matrice->rows * i + j) =
          *(matrice_to_copy->array_core + matrice->rows * i + j);
    }
  }
};
int find_unique_xor(int arr[3]) { return arr[0] ^ arr[1] ^ arr[2]; };

Matrice apply_ghosting(Matrice *true_state) {
  int ghost_row;
  int ghost_col;
  Matrice observed_state;
  indices_array rectangle;
  rectangle.size = 0;
  rectangle.capacity = 12;
  rectangle.array_core = (Index *)malloc(rectangle.capacity * sizeof(Index));
  copy_to(true_state, &observed_state);

  for (int i = 0; i < observed_state.lines; i++) {
    for (int j = 0; j < observed_state.rows; j++) {
      int element = *(observed_state.array_core + observed_state.rows * i + j);
      if (element == 1) {
        Index index;
        index.i = i;
        index.j = j;
        add_element(&rectangle, index);
      }
    }
  }

  // end for for
  Index p1, p2, p3;
  int N = rectangle.size;
  for (int a = 0; a < N; a++) {
    for (int b = a + 1; b < N;
         b++) { // Start at a+1 to avoid duplicates like (0,0,1) and (0,1,0)
      for (int c = b + 1; c < N; c++) { // Start at b+1 for the same reason
        // Now you have three distinct indices from the list:
        p1 = rectangle.array_core[a]; // e.g., (r1, c1)
        p2 = rectangle.array_core[b]; // e.g., (r2, c2)
        p3 = rectangle.array_core[c]; // e.g., (r3, c3)

        // The next steps happen inside this innermost loop...
        int rows[3] = {p1.i, p2.i, p3.i};
        int cols[3] = {p1.j, p2.j, p3.j};

        // Check if there are exactly 2 unique rows and 2 unique columns
        // A simple way is to check if one row is repeated, and one column is
        // repeated
        int is_rect_triangle =
            (rows[0] == rows[1] || rows[0] == rows[2] || rows[1] == rows[2]) &&
            (cols[0] == cols[1] || cols[0] == cols[2] || cols[1] == cols[2]);

        if (is_rect_triangle) {
          ghost_row = find_unique_xor(rows);
          ghost_col = find_unique_xor(cols);
          observed_state
              .array_core[ghost_row * observed_state.rows + ghost_col] = 1;
        }
      }
    }
  }
  free(rectangle.array_core);
  return observed_state;
};
