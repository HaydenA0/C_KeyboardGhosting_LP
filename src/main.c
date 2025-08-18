#include "matrice.h" // Includes the necessary matrix definitions and function prototypes
#include <stdio.h>
#include <stdlib.h>

// Helper functions to print the matrices for visualization
void print_int_matrix(const char *title, Matrice *m) {
  if (m == NULL || m->array_core == NULL) {
    printf("Cannot print invalid integer matrix: %s\n", title);
    return;
  }
  printf("--- %s ---\n", title);
  for (unsigned int i = 0; i < m->lines; i++) {
    for (unsigned int j = 0; j < m->rows; j++) {
      // Print with a sign to make -1 more visible
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
  // --- Setup ---
  const unsigned int LINES = 5;
  const unsigned int ROWS = 6;
  const size_t matrix_size = LINES * ROWS;

  // The character map (keyboard layout)
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

  // --- Step 1: Set up the scenario in main ---

  // Initialize the first snapshot: state_t0
  Matrice state_t0;
  state_t0.lines = LINES;
  state_t0.rows = ROWS;
  state_t0.array_core = (int *)calloc(matrix_size, sizeof(int));

  // At time t0, keys '2' and 'F' are pressed.
  state_t0.array_core[0 * ROWS + 1] =
      1; // Press '2' at (0, 1) -> This will be released
  state_t0.array_core[2 * ROWS + 3] =
      1; // Press 'F' at (2, 3) -> This will be held
  print_int_matrix("Initial State (t0)", &state_t0);

  // Initialize the second snapshot: state_t1
  Matrice state_t1;
  state_t1.lines = LINES;
  state_t1.rows = ROWS;
  state_t1.array_core = (int *)calloc(matrix_size, sizeof(int));

  // At time t1, key '2' is released, 'F' is held, and 'W' is newly pressed.
  state_t1.array_core[2 * ROWS + 3] = 1; // 'F' is still held
  state_t1.array_core[1 * ROWS + 1] = 1; // 'W' is newly pressed at (1, 1)
  print_int_matrix("Next State (t1)", &state_t1);

  // --- Step 2: The Core Calculation ---
  printf("Calculating difference: state_t1 - state_t0...\n\n");
  Matrice delta_matrix = m_difference(&state_t1, &state_t0);

  // Crucial check: m_difference returns a matrix with a NULL core on failure.
  if (delta_matrix.array_core == NULL) {
    fprintf(stderr, "Error: m_difference failed. Matrices might have different "
                    "dimensions or memory allocation failed.\n");
    // Cleanup before exiting
    free(state_t0.array_core);
    free(state_t1.array_core);
    free(char_table.array_core);
    return 1;
  }
  print_int_matrix("Delta Matrix (t1 - t0)", &delta_matrix);

  // --- Step 3 & 4: Interpreting the Result and Reporting ---
  printf("--- Analysis of Key Changes ---\n");
  for (unsigned int i = 0; i < delta_matrix.lines; i++) {
    for (unsigned int j = 0; j < delta_matrix.rows; j++) {
      int value = i_get_element(&delta_matrix, i, j);

      if (value == 1) { // A 1 means a new key press (1 - 0 = 1)
        char pressed_char = c_get_element(&char_table, i, j);
        printf("Key Pressed:  '%c' at (%u, %u)\n", pressed_char, i, j);
      } else if (value == -1) { // A -1 means a key release (0 - 1 = -1)
        char released_char = c_get_element(&char_table, i, j);
        printf("Key Released: '%c' at (%u, %u)\n", released_char, i, j);
      }
      // A value of 0 means no change, so we do nothing.
    }
  }
  printf("-------------------------------\n\n");

  // --- Final Cleanup ---
  free(state_t0.array_core);
  free(state_t1.array_core);
  free(delta_matrix.array_core);
  free(char_table.array_core);

  printf("Program finished successfully.\n");
  return 0;
}
