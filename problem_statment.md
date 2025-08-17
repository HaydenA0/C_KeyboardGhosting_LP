
### 1. The Pinned Location

> **Scanning procedure**
>
> Computer keyboards include control circuitry to convert key presses into key codes (usually scancodes) that the computer's electronics can understand. The key switches are connected via the printed circuit board in an electrical X-Y matrix where a voltage is provided sequentially to the Y lines and, when a key is depressed, detected sequentially by scanning the X lines.

And for a more advanced version of the problem:

> But when three keys are pressed (electrically closed) at the same time in a "direct contact" keyswitch matrix that doesn't have isolation diodes, the keyboard electronics can see a fourth "phantom" key which is the intersection of the X and Y lines of the three keys.

### 2. The Justification

This concept is an ideal candidate for a **Low-Level Programming** challenge. It asks you to simulate the fundamental logic of a piece of hardware—the keyboard's control processor.

*   **No External Libraries Needed:** The core problem can be solved with basic data structures (arrays/matrices) and control flow (loops), making it a pure test of algorithmic thinking.
*   **Real-World Application:** You are modeling a process that happens billions of times a day in devices all over the world. This provides a tangible connection between the code and a physical system.
*   **Scalable Complexity:** The problem can be made progressively more difficult by introducing real-world hardware limitations like "debouncing" and "phantom keys," which are mentioned in the article.

### 3. The Programming Problem

You will write a program that simulates a keyboard controller. Your task is to scan a virtual key matrix and report which keys have been pressed.

---

#### **Level 1: Basic Matrix Scan**

**Objective:** Implement a function that scans a keyboard matrix and identifies which single key is currently pressed.

**Details:**
1.  **Input:**
    *   A 2D array (matrix) representing the state of the keyboard switches. `0` means the key is not pressed, and `1` means it is pressed.
    *   A key map that associates a matrix coordinate `(row, col)` with a character or "scancode" (e.g., `(0, 0)` -> `"Q"`, `(0, 1)` -> `"W"`).

2.  **Task:**
    *   Write a function `scanMatrix(matrix, keymap)` that simulates the scanning process.
    *   The function should iterate through each row. For each row, it should iterate through each column to check for a `1`.
    *   If a `1` is found, the function should use the `keymap` to find the corresponding character and return it.
    *   If no key is pressed (the matrix is all `0`s), it should return `null` or an empty string.

**Example:**

```
// Simplified 2x3 matrix for A, S, D, Z, X, C keys
const keymap = {
  "0,0": "A", "0,1": "S", "0,2": "D",
  "1,0": "Z", "1,1": "X", "1,2": "C"
};

// 'S' key is pressed
const keyMatrix = [
  [0, 1, 0],
  [0, 0, 0]
];

scanMatrix(keyMatrix, keymap) // Should return "S"
```

---

#### **Level 2: Handling Key Press and Release Events**

**Objective:** Modify your scanner to track state. Real keyboards don't just say "S is down"; they emit distinct "key down" and "key up" events.

**Details:**
1.  **Task:**
    *   Create a `KeyboardController` class or object that holds the `previousMatrixState`.
    *   Its `scan()` method should now compare the `currentMatrixState` with the `previousMatrixState`.
    *   The method should return a list of events that occurred between the last scan and this one. An event could be an object like `{ key: "S", type: "keydown" }` or `{ key: "S", type: "keyup" }`.
    *   After each scan, update `previousMatrixState` to the `currentMatrixState`.

**Example:**

```
// Controller is initialized with an all-zero matrix
let controller = new KeyboardController(keymap);

// First scan: 'S' is pressed
let matrix1 = [[0, 1, 0], [0, 0, 0]];
controller.scan(matrix1) // Should return [{ key: "S", type: "keydown" }]

// Second scan: 'S' is still held down, 'X' is also pressed
let matrix2 = [[0, 1, 0], [0, 1, 0]];
controller.scan(matrix2) // Should return [{ key: "X", type: "keydown" }]

// Third scan: 'S' is released
let matrix3 = [[0, 0, 0], [0, 1, 0]];
controller.scan(matrix3) // Should return [{ key: "S", type: "keyup" }]
```

---

#### **Level 3: Detecting Phantom Keys**

**Objective:** Simulate the hardware limitation of "phantom keys" found in simpler keyboard matrices without isolation diodes.

**Details:**
1.  **The Concept:** As the article states, if you press three keys that form a rectangle with a fourth, unpressed key, the circuit can be fooled into thinking that fourth key is also pressed. For example, if keys at `(r1, c1)`, `(r1, c2)`, and `(r2, c1)` are pressed, a phantom key might appear at `(r2, c2)`.

2.  **Task:**
    *   Write a function `detectPhantomKeys(pressedKeys)` where `pressedKeys` is a list of coordinate pairs `[row, col]` for all actively pressed keys.
    *   This function should analyze the list of pressed keys to identify any "phantom keys" that would be erroneously detected by the circuitry.
    *   It should return a list of the coordinates of the detected phantom keys.

**Example:**

```
// User is actually pressing 'W', 'E', and 'S'
// on a standard QWERTY layout.
// 'W' is at (0,1), 'E' is at (0,2), 'S' is at (1,1)
let actualPresses = [[0, 1], [0, 2], [1, 1]];

// These three keys form a rectangle with the key at (1,2), which is 'D'.
// 'D' is the phantom key.
detectPhantomKeys(actualPresses) // Should return [[1, 2]]

// Bonus Challenge:
// Modify your `KeyboardController` from Level 2.
// When multiple keys are down, it should first run `detectPhantomKeys`.
// It should then implement a "phantom key blocking" strategy. A simple one is:
// if a phantom key is detected, report NONE of the keys that caused it,
// simulating how a cheap keyboard might "lock up" in this state.
```
