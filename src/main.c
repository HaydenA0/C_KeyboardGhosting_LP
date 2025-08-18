#include "matrice.h"
#include <stdio.h>
#include <stdlib.h>

void print_int_matrix(Matrice *m) {
  if (m == NULL || m->array_core == NULL) {
    printf("Cannot print invalid integer matrix.\n");
    return;
  }
  for (unsigned int i = 0; i < m->lines; i++) {
    for (unsigned int j = 0; j < m->rows; j++) {
      printf("%d ", m->array_core[i * m->rows + j]);
    }
    printf("\n");
  }
}

void print_char_matrix(C_Matrice *m) {
  if (m == NULL || m->array_core == NULL) {
    printf("Cannot print invalid char matrix.\n");
    return;
  }
  for (unsigned int i = 0; i < m->lines; i++) {
    for (unsigned int j = 0; j < m->rows; j++) {
      printf("%c ", m->array_core[i * m->rows + j]);
    }
    printf("\n");
  }
}

int main() {
  Matrice circuit_mask;
  circuit_mask.lines = 5;
  circuit_mask.rows = 6;
  size_t matrix_size = circuit_mask.lines * circuit_mask.rows;

  circuit_mask.array_core = (int *)calloc(matrix_size, sizeof(int));
  if (circuit_mask.array_core == NULL) {
    perror("Failed to allocate memory for the circuit mask");
    return 1;
  }

  // Set the 3 "pressed keys" by changing elements from 0 to 1
  // Let's press the keys at (0, 1), (2, 3), and (4, 5)
  circuit_mask.array_core[0 * circuit_mask.rows + 1] = 1; // Corresponds to '2'
  circuit_mask.array_core[2 * circuit_mask.rows + 3] = 1; // Corresponds to 'F'
  circuit_mask.array_core[4 * circuit_mask.rows + 5] = 1; // Corresponds to '^'

  // 2. Initialize a 5x6 character matrix (the layout table)
  C_Matrice char_table;
  char_table.lines = 5;
  char_table.rows = 6;
  char_table.array_core = (char *)malloc(matrix_size * sizeof(char));
  if (char_table.array_core == NULL) {
    perror("Failed to allocate memory for the character table");
    free(circuit_mask.array_core); // Clean up previous allocation
    return 1;
  }

  // A sample keyboard layout
  char layout[5][6] = {{'1', '2', '3', '4', '5', '6'},
                       {'Q', 'W', 'E', 'R', 'T', 'Y'},
                       {'A', 'S', 'D', 'F', 'G', 'H'},
                       {'Z', 'X', 'C', 'V', 'B', 'N'},
                       {'!', '@', '#', '$', '%', '^'}};

  // Copy the 2D layout into our 1D array_core
  for (unsigned int i = 0; i < char_table.lines; i++) {
    for (unsigned int j = 0; j < char_table.rows; j++) {
      char_table.array_core[i * char_table.rows + j] = layout[i][j];
    }
  }

  // --- Visualization ---
  printf("Circuit Mask (where 1 indicates a key press):\n");
  print_int_matrix(&circuit_mask);
  printf("\nCharacter Table:\n");
  print_char_matrix(&char_table);
  printf("\n----------------------------------------\n");

  // 3. Run the function to get the pressed keys
  char *keys_pressed = find_masked_chars(&circuit_mask, &char_table);

  // 4. Print the result and clean up
  if (keys_pressed != NULL) {
    printf("Result: The characters at the masked positions are: \"%s\"\n",
           keys_pressed);
    // Clean up the string returned by the function
    free(keys_pressed);
  } else {
    printf("The function returned NULL, an error may have occurred.\n");
  }

  printf("----------------------------------------\n\n");

  // Free the matrices' memory
  free(circuit_mask.array_core);
  free(char_table.array_core);

  printf("All memory has been freed. Program finished.\n");

  return 0;
}
