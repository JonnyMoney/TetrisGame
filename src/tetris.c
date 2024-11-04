#include <stdio.h>
#include <stdlib.h>	 // for rand() and srand()
#include <time.h>	 // for time()
#include <stdbool.h> // for bool to make the code more readable
#include <conio.h>	 // for _kbhit() and _getch() on Windows
#include <windows.h> // for Sleep and clearing part of the board

#define BOARDROWS 20
#define BOARDCOLS 20		   // The BOARDCOLS needs to be even, and if you want to change this size remember to change the template at the CreateBoard Function
#define MOVE_DOWN_INTERVAL 500 // 500 milliseconds

typedef struct
{
	int x;
	int y;
} Point;

Point CurrentPos = {((BOARDCOLS / 2) - 2), 0};
int currentrotation;

void resetPosition()
{
	CurrentPos.x = (BOARDCOLS / 2) - 2;
	CurrentPos.y = 0;
}

typedef enum
{
	BLOCK_2X2,
	BLOCK_3x3,
	BLOCK_4x4
} BlockType;

typedef struct
{
	BlockType type;
	int rows;
	int cols;
	char block[4][4]; // The largest block is 4x4, so we reserve enough space.
	bool is_mooving;

} TetrisBlock;

void CreateBoard(char emptyboard[BOARDROWS][BOARDCOLS]);
void PrintBoard(char board[BOARDROWS][BOARDCOLS]);
void ClearShapeOffBoard(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block);
void PutShapeOnBoard(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block);
TetrisBlock *PutRandomShapeOnBoard(char board[BOARDROWS][BOARDCOLS], TetrisBlock *blocks[]);

bool MoveShapeDown(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block);
bool CanMoveDown(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block);

void RotateShape(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block);
void ResetBlock(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block);

bool MoveShapeRight(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block);
bool CanMoveRight(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block);

bool MoveShapeLeft(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block);
bool CanMoveLeft(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block);

int ClearFullRows(char board[BOARDROWS][BOARDCOLS]);

int main()
{
	system("cls");
	char board[BOARDROWS][BOARDCOLS];
	CreateBoard(board);

	TetrisBlock oBlock = {
		.type = BLOCK_2X2,
		.rows = 2,
		.cols = 2,
		.block = {
			{'#', '#'},
			{'#', '#'}},
		.is_mooving = false};

	TetrisBlock iBlock = {
		.type = BLOCK_4x4,
		.rows = 4,
		.cols = 4,
		.block = {
			{'.', '#', '.', '.'},
			{'.', '#', '.', '.'},
			{'.', '#', '.', '.'},
			{'.', '#', '.', '.'}},
		.is_mooving = false};

	TetrisBlock lBlock = {
		.type = BLOCK_3x3,
		.rows = 3,
		.cols = 3,
		.block = {
			{'.', '#', '.'},
			{'.', '#', '.'},
			{'.', '#', '#'}},
		.is_mooving = false};

	TetrisBlock jBlock = {
		.type = BLOCK_3x3,
		.rows = 3,
		.cols = 3,
		.block = {
			{'.', '#', '.'},
			{'.', '#', '.'},
			{'#', '#', '.'}},
		.is_mooving = false};

	TetrisBlock tBlock = {
		.type = BLOCK_3x3,
		.rows = 3,
		.cols = 3,
		.block = {
			{'.', '.', '.'},
			{'#', '#', '#'},
			{'.', '#', '.'}},
		.is_mooving = false};

	TetrisBlock zBlock = {
		.type = BLOCK_3x3,
		.rows = 3,
		.cols = 3,
		.block = {
			{'.', '.', '.'},
			{'#', '#', '.'},
			{'.', '#', '#'}},
		.is_mooving = false};

	TetrisBlock sBlock = {
		.type = BLOCK_3x3,
		.rows = 3,
		.cols = 3,
		.block = {
			{'.', '.', '.'},
			{'.', '#', '#'},
			{'#', '#', '.'}},
		.is_mooving = false};
	TetrisBlock *blocks[] = {&oBlock, &iBlock, &lBlock, &jBlock, &tBlock, &zBlock, &sBlock};
	int score = 0;
	srand(time(0)); // To make sure the rand function wont be the sequence after losing
	while (true)
	{
		TetrisBlock *currentBlock = PutRandomShapeOnBoard(board, blocks);

		clock_t lastMoveTime = clock(); // Initialize the timer

		// Check if the game is over:
		if (!CanMoveDown(board, currentBlock) && CurrentPos.y <= 2)
		{
			system("cls");
			printf("GAME OVER\n");
			printf("SCORE: %d\n", score);
			score = 0; // Initilize the score
			break;
		}
		while (true)
		{ // Loop to move the shape down
			// Check for user input

			if (_kbhit())
			{
				char ch = _getch();
				switch (ch)
				{
				case 'a':
					MoveShapeLeft(board, currentBlock);
					break;
				case 'd':
					MoveShapeRight(board, currentBlock);
					break;
				case 'w':
					ClearShapeOffBoard(board, currentBlock);
					RotateShape(board, currentBlock);
					if (!CanMoveLeft(board, currentBlock) || !CanMoveRight(board, currentBlock))
					{
						for (int i = 0; i < 3; i++)
						{
							RotateShape(board, currentBlock);
						}
					}
					PutShapeOnBoard(board, currentBlock);
					break;
				case 's':
					// Move the shape down immediately
					if (MoveShapeDown(board, currentBlock))
					{
						PrintBoard(board); // Print the updated board
					}
					break;
				case 'q':
					exit(0); // Quit the game
				}
				PrintBoard(board); // Print the board after any manual movement
			}

			// Move the shape down after 0.5 seconds
			double elapsedTime = (double)(clock() - lastMoveTime) / CLOCKS_PER_SEC * 1000; // convert to milliseconds
			if (elapsedTime >= MOVE_DOWN_INTERVAL)
			{
				if (MoveShapeDown(board, currentBlock))
				{
					PrintBoard(board);		// Print the updated board
					lastMoveTime = clock(); // Reset the timer
				}
				else
				{
					break; // Exit if the shape cannot move down anymore
				}
			}

			Sleep(10); // Yield to prevent high CPU usage
		}

		ResetBlock(board, currentBlock);
		score += ClearFullRows(board);
	}
	return 0;
}

void CreateBoard(char emptyboard[BOARDROWS][BOARDCOLS])
{
	char *template = "<i................i>"; // if BOARDCOLS is changed needs tgo be changed
	for (int i = 0; i < BOARDROWS - 1; i++)
	{
		for (int j = 0; j < BOARDROWS; j++)
		{
			emptyboard[i][j] = template[j];
		}
	}

	for (int i = 2; i < BOARDCOLS - 2; i++)
	{
		emptyboard[BOARDROWS - 2][i] = '=';
	}

	emptyboard[BOARDROWS - 1][0] = ' ';
	emptyboard[BOARDROWS - 1][1] = ' ';
	emptyboard[BOARDROWS - 1][BOARDCOLS - 2] = ' ';
	emptyboard[BOARDROWS - 1][BOARDCOLS - 1] = ' ';
	for (int i = 2; i < BOARDCOLS - 2; i++)
	{
		emptyboard[BOARDROWS - 1][i] = 'V';
	}
}

void PrintBoard(char board[BOARDROWS][BOARDCOLS])
{
	COORD coord;
	coord.X = 0;
	coord.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

	for (int i = 0; i < BOARDROWS; i++)
	{
		for (int j = 0; j < BOARDCOLS; j++)
		{
			printf("%c", board[i][j]);
		}
		printf("\n");
	}
}

void ClearShapeOffBoard(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block)
{
	/*
	 * Just goes over the shape and only deletes the '#' so it wont delete any other shape n its radius
	 */
	for (int i = 0; i < block->rows; i++)
	{
		for (int j = 0; j < block->cols; j++)
		{
			if (block->block[i][j] == '#')
			{
				board[CurrentPos.y + i][CurrentPos.x + j] = '.';
			}
		}
	}
}

void PutShapeOnBoard(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block)
{
	/*
	 * Just goes over the shape and only puts the '#' where the shape has a '#' so it wont delete any other shape in its radius
	 */
	for (int i = 0; i < block->rows; i++)
	{
		for (int j = 0; j < block->cols; j++)
		{
			if (block->block[i][j] == '#')
			{
				board[CurrentPos.y + i][CurrentPos.x + j] = '#';
			}
		}
	}
}

TetrisBlock *PutRandomShapeOnBoard(char board[BOARDROWS][BOARDCOLS], TetrisBlock *blocks[])
{
	resetPosition();
	int randnum = rand() % 7;
	TetrisBlock *selectedBlock = blocks[randnum];

	CurrentPos.y = 0; // To set the position to be the top of the board.

	for (int i = 0; i < selectedBlock->rows; i++)
	{
		for (int j = 0; j < selectedBlock->cols; j++)
		{
			board[CurrentPos.y + i][CurrentPos.x + j] = selectedBlock->block[i][j];
		}
	}
	selectedBlock->is_mooving = true;
	return selectedBlock;
}

bool MoveShapeDown(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block)
{
	// Check collision for every block element
	if (!CanMoveDown(board, block))
	{
		return false;
	}

	// Clear current position
	ClearShapeOffBoard(board, block);

	// Update position
	CurrentPos.y++;

	// Place block in new position
	PutShapeOnBoard(board, block);

	return true;
}

bool CanMoveDown(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block)
{

	for (int col = 0; col < block->cols; col++) // Checking the bottom layer.
	{
		if (block->block[block->rows - 1][col] == '#')
		{
			if (board[CurrentPos.y + block->rows][CurrentPos.x + col] != '.')
			{
				return false;
			}
		}
	}

	for (int row = 0; row < block->rows - 1; row++) // minus 1 because we dont need to check the last row (checked in the first loop)
	{
		for (int col = 0; col < block->cols; col++)
		{
			if (block->block[row][col] == '#' && block->block[row + 1][col] == '.') // if the currnet char is '#' and the char under it is '.'
			{
				if (board[CurrentPos.y + row + 1][CurrentPos.x + col] != '.')
				{
					return false;
				}
			}
		}
	}
	return true;
}

void RotateShape(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block)
{
	// Mirror by the diagonal
	for (int row = 0; row < block->rows; row++)
	{
		for (int col = row; col < block->cols; col++)
		{
			char temp = block->block[row][col]; // saving temp of current location
			block->block[row][col] = block->block[col][row];
			block->block[col][row] = temp;
		}
	}

	int length = block->cols;
	// Mirror by the middle
	for (int row = 0; row < block->rows; row++)
	{
		for (int col = 0; col < block->cols / 2; col++)
		{
			char temp = block->block[row][col]; // saving temp of current location
			block->block[row][col] = block->block[row][length - col - 1];
			block->block[row][length - col - 1] = temp;
		}
	}
	currentrotation++;
}

void ResetBlock(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block)
{
	while (currentrotation % 4 != 0)
	{
		RotateShape(board, block);
	}
}

bool MoveShapeLeft(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block)
{
	if (!CanMoveLeft(board, block))
	{
		return false;
	}

	// Clear current position
	ClearShapeOffBoard(board, block);

	// Update position
	CurrentPos.x--;

	// Place block in new position
	PutShapeOnBoard(board, block);

	return true;
}

bool CanMoveLeft(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block)
{
	// Check the leftmost column of the block
	for (int row = 0; row < block->rows; row++)
	{
		if (block->block[row][0] == '#')
		{
			if (board[CurrentPos.y + row][CurrentPos.x - 1] != '.')
			{
				return false;
			}
		}
	}

	// Check the rest of the block
	for (int col = 1; col < block->cols; col++) // start from 1 because we already checked the first column
	{
		for (int row = 0; row < block->rows; row++)
		{
			if (block->block[row][col] == '#' && block->block[row][col - 1] == '.') // if current char is '#' and char to the left is '.'
			{
				if (board[CurrentPos.y + row][CurrentPos.x + col - 1] != '.')
				{
					return false;
				}
			}
		}
	}

	return true;
}

bool MoveShapeRight(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block)
{
	if (!CanMoveRight(board, block))
	{
		return false;
	}

	// Clear current position
	ClearShapeOffBoard(board, block);

	// Update position
	CurrentPos.x++;

	// Place block in new position
	PutShapeOnBoard(board, block);

	return true;
}

bool CanMoveRight(char board[BOARDROWS][BOARDCOLS], TetrisBlock *block)
{
	// Check the rightmost column of the block
	for (int row = 0; row < block->rows; row++)
	{
		if (block->block[row][block->cols - 1] == '#')
		{
			if (board[CurrentPos.y + row][CurrentPos.x + block->cols] != '.')
			{
				return false;
			}
		}
	}

	// Check the rest of the block
	for (int col = 0; col < block->cols - 1; col++)
	{ // minus 1 because we don’t need to check the last column
		for (int row = 0; row < block->rows; row++)
		{
			if (block->block[row][col] == '#' && block->block[row][col + 1] == '.')
			{ // if current char is '#' and char to the right is '.'
				if (board[CurrentPos.y + row][CurrentPos.x + col + 1] != '.')
				{
					return false;
				}
			}
		}
	}

	return true;
}

int ClearFullRows(char board[BOARDROWS][BOARDCOLS])
{
	int clear_rows_counter = 0;
	for (int row = 2; row < BOARDROWS - 2; row++)
	{
		bool isFull = true;
		for (int col = 2; col < BOARDCOLS - 2; col++)
		{
			if (board[row][col] != '#')
			{
				isFull = false;
				break;
			}
		}

		if (isFull)
		{
			clear_rows_counter++;
			// Clear the row
			for (int col = 2; col < BOARDCOLS - 2; col++)
			{
				board[row][col] = '.';
			}

			// Move all rows above down by one
			for (int moveRow = row; moveRow > 2; moveRow--) // goes up from the current row, and sets whatever was there to be one row down.
			{
				for (int col = 2; col < BOARDCOLS - 2; col++)
				{
					board[moveRow][col] = board[moveRow - 1][col];
				}
			}

			// Clear the top visible row
			for (int col = 2; col < BOARDCOLS - 2; col++)
			{
				board[2][col] = '.';
			}

			row--; // Check the same row again since it now contains the row above
		}
	}
	return clear_rows_counter * 10;
}
