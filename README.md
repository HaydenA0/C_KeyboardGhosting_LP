### 0. Demonstration

*(To be added)*

### 1. General information

-   **Name of the project**: Keyboard Ghosting and State Analysis
-   **Type of the project**: Personal Learning
-   **Main Language(s) of the project**: C
-   **Goal of this project**: The main goal of this project was to create a small C library to represent keyboard layouts and key presses using matrices. It also aims to simulate and analyze keyboard ghosting, a phenomenon where pressing multiple keys simultaneously can register an additional, unpressed key.
-   **Scope of this project**: This project includes custom data structures for a dynamic array and a 2D matrix, along with functions to perform operations like calculating state differences, finding pressed keys, and simulating the ghosting effect. The `main.c` file serves as a demonstration of some of these features.
-   **Compatibility**: The project is written in standard C and uses common libraries (`stdio.h`, `stdlib.h`). It should compile and run on any system with a standard C compiler, such as GCC. I have tested it on a Linux-based environment.

### 2. Project

This project implements a few core components to model and analyze keyboard states.

I created two main data structures from scratch. The first is a dynamic array (`indices_array`) designed to hold 2D coordinates. It manages its own memory by tracking its size and capacity, and I wrote a `resize_array` function using `realloc` to automatically double its capacity when it becomes full.

The second structure is a 2D matrix, which I implemented for both integers (`Matrice`) and characters (`C_Matrice`). To keep the implementation simple, I represented the 2D grid as a flat, one-dimensional array in memory. The matrix struct stores the number of lines and rows, and I used simple pointer arithmetic (`i * rows + j`) to access the correct element.

The core logic of the project uses these matrices to model a keyboard. A `C_Matrice` holds the keyboard layout, while an `int` `Matrice` represents the state of pressed keys (1 for pressed, 0 for not pressed). I wrote a function, `m_difference`, which performs an element-wise subtraction of two state matrices to find changes over time. A resulting value of `1` indicates a new key press, and `-1` indicates a key release.

Finally, I implemented a function `apply_ghosting` to simulate keyboard ghosting. My approach was to find all currently pressed keys and iterate through every combination of three. If a set of three pressed keys forms the corners of a rectangle on the grid, the function calculates the coordinates of the fourth corner and artificially marks it as pressed in an "observed state" matrix.

### 3. How to run the project :

To compile and run this project, you will need a C compiler like GCC installed on your system.

1.  Make sure all the source files (`.c` and `.h`) are placed inside the `src` directory as described in the structure.

2.  Navigate to the `src` directory in your terminal:
    ```sh
    cd src
    ```

3.  The provided `run.sh` script is designed to compile and execute the program. First, make it executable:
    ```sh
    chmod +x run.sh
    ```

4.  Then, run the script:
    ```sh
    ./run.sh
    ```

This script will compile all the `.c` files into a single executable and then run it. The output from the `main.c` demonstration will be printed to your terminal.

Alternatively, you can compile the files manually with GCC and run the output:
```sh
gcc main.c matrice.c custom_array.c -o program && ./program
```
