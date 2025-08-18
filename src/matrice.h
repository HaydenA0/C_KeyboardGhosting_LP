#ifndef MATRICE_H
#define MATRICE_H

typedef struct Matrice {
  int *array_core;
  unsigned int lines; // lines cant be negative
  unsigned int rows;  // same here
} Matrice;

typedef struct C_Matrice {
  char *array_core;
  unsigned int lines; // lines cant be negative
  unsigned int rows;  // same here
} C_Matrice;

int assert_matrice(Matrice *matrice);
int assert_c_matrice(C_Matrice *matrice);
char *find_masked_chars(Matrice *mask,
                        C_Matrice *data); // Kept the same Matrice name
Matrice m_difference(Matrice *matrice1, Matrice *matrice2);
int i_get_element(Matrice *matrice, int i, int j);
char c_get_element(C_Matrice *matrice, int i, int j);
void copy_to(Matrice *matrice_to_copy, Matrice *matrice);
Matrice apply_ghosting(Matrice *true_state);

#endif // !MATRICE_H
