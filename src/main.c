#include "matrice.h"
#include <stdio.h>
#include <stdlib.h>

void print_int_matrix(const char *title, Matrice *m) {
  if (m == NULL || m->array_core == NULL) {
    printf("Cannot print invalid integer matrix: %s\n", title);
    return;
  }
  printf("--- %s ---\n", title);
  for (unsigned int i = 0; i < m->lines; i++) {
    for (unsigned int j = 0; j < m->rows; j++) {

      printf("%+2d ", m->array_core[i * m->rows + j]);
    }
    printf("\n");
  }
  printf("\n");
}

void print_char_matrix(const char *title, C_Matrice *m) {
  if (m == NULL || m->array_core == NULL) {
    printf("Cannot print invalid char matrix: %s\n", title);
    return;
  }
  printf("--- %s ---\n", title);
  for (unsigned int i = 0; i < m->lines; i++) {
    for (unsigned int j = 0; j < m->rows; j++) {
      printf("%c ", m->array_core[i * m->rows + j]);
    }
    printf("\n");
  }
  printf("\n");
}

int main() {

  const unsigned int LINES = 5;
  const unsigned int ROWS = 6;
  const size_t matrix_size = LINES * ROWS;

  C_Matrice char_table;
  char_table.lines = LINES;
  char_table.rows = ROWS;
  char_table.array_core = (char *)malloc(matrix_size * sizeof(char));
  if (char_table.array_core == NULL) {
    perror("Failed to allocate memory for the character table");
    return 1;
  }

  char layout[5][6] = {{'1', '2', '3', '4', '5', '6'},
                       {'Q', 'W', 'E', 'R', 'T', 'Y'},
                       {'A', 'S', 'D', 'F', 'G', 'H'},
                       {'Z', 'X', 'C', 'V', 'B', 'N'},
                       {'!', '@', '#', '$', '%', '^'}};

  for (unsigned int i = 0; i < LINES; i++) {
    for (unsigned int j = 0; j < ROWS; j++) {
      char_table.array_core[i * ROWS + j] = layout[i][j];
    }
  }
  print_char_matrix("Keyboard Layout", &char_table);

  Matrice state_t0;
  state_t0.lines = LINES;
  state_t0.rows = ROWS;
  state_t0.array_core = (int *)calloc(matrix_size, sizeof(int));

  state_t0.array_core[0 * ROWS + 1] = 1;
  state_t0.array_core[2 * ROWS + 3] = 1;
  print_int_matrix("Initial State (t0)", &state_t0);

  Matrice state_t1;
  state_t1.lines = LINES;
  state_t1.rows = ROWS;
  state_t1.array_core = (int *)calloc(matrix_size, sizeof(int));

  state_t1.array_core[2 * ROWS + 3] = 1;
  state_t1.array_core[1 * ROWS + 1] = 1;
  print_int_matrix("Next State (t1)", &state_t1);

  printf("Calculating difference: state_t1 - state_t0...\n\n");
  Matrice delta_matrix = m_difference(&state_t1, &state_t0);

  if (delta_matrix.array_core == NULL) {
    fprintf(stderr, "Error: m_difference failed. Matrices might have different "
                    "dimensions or memory allocation failed.\n");

    free(state_t0.array_core);
    free(state_t1.array_core);
    free(char_table.array_core);
    return 1;
  }
  print_int_matrix("Delta Matrix (t1 - t0)", &delta_matrix);

  printf("--- Analysis of Key Changes ---\n");
  for (unsigned int i = 0; i < delta_matrix.lines; i++) {
    for (unsigned int j = 0; j < delta_matrix.rows; j++) {
      int value = i_get_element(&delta_matrix, i, j);

      if (value == 1) {
        char pressed_char = c_get_element(&char_table, i, j);
        printf("Key Pressed:  '%c' at (%u, %u)\n", pressed_char, i, j);
      } else if (value == -1) {
        char released_char = c_get_element(&char_table, i, j);
        printf("Key Released: '%c' at (%u, %u)\n", released_char, i, j);
      }
    }
  }
  printf("-------------------------------\n\n");

  free(state_t0.array_core);
  free(state_t1.array_core);
  free(delta_matrix.array_core);
  free(char_table.array_core);

  printf("Program finished successfully.\n");
  return 0;
}
