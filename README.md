# Tetris Game in C

## Overview
This is a simple console-based Tetris game implemented in C. Players can control falling tetromino shapes using keyboard inputs to fill rows and score points. The game runs in a terminal and provides a fun way to experience classic gameplay.

## Features
- Randomly generated tetromino shapes (O, I, L, J, T, Z, S).
- Keyboard controls for moving and rotating shapes:
  - `a`: Move left
  - `d`: Move right
  - `w`: Rotate
  - `s`: Accelerate down
  - `q`: Quit the game
- Scoring system based on completed rows.
- Game over detection when shapes stack to the top.

## Installation
1. Make sure you have a C compiler installed (like GCC).
2. Clone the repository or download the source code files.
3. Open a terminal and navigate to the project directory.
4. Compile the program using the following commands:
   - **Linux:**
     ```bash
     gcc tetris.c -o tetris -lm
     ```
   - **Windows:**
     ```bash
     gcc tetris.c -o tetris.exe
     ```
5. Run the game:
   - **Linux:**
     ```bash
     ./tetris
     ```
   - **Windows:**
     ```bash
     tetris.exe
     ```

Try to fill complete rows with the falling shapes to score points!

## Code Structure
- `main()`: Initializes the game, creates the board, and runs the main game loop.
- `CreateBoard()`: Sets up the game board.
- `PrintBoard()`: Displays the current state of the board.
- `MoveShapeDown()`, `CanMoveDown()`, `MoveShapeLeft()`, `MoveShapeRight()`: Functions for controlling shape movement and collision detection.
- `RotateShape()`: Handles rotation of the current shape.
- `ClearFullRows()`: Clears completed rows and updates the score.

## License
This project is open-source and available for anyone to use and modify. Enjoy coding and gaming!
