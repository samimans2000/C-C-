//
// Created by noamfi on 8/10/18.
//
/**
* @section DESCRIPTION
* The program handles the game flow of the game
* Input  : the turns and the size of the board
* Output : the game play
*/

// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "battleships.h"

// ------------------------------ const definitions ------------------------------
/**
 * @def IN_BOARD_SIZE 5
 * @brief the minimum board size allowed
 */
#define MIN_BOARD_SIZE 5
/**
 * @def MAX_BOARD_SIZE 26
 * @brief the maximum board size allowed
 */
#define MAX_BOARD_SIZE 26

// ------------------------------ structs and functions ------------------------------
/**
 * @brief receives the input from the user after first clearing away the newline character from the stream
 * @param input a pointer that points to the place we will hold the user input
 */
void getInput (int BUFFER_SIZE , char* input)
{
	fgets (input , BUFFER_SIZE , stdin);
	scanf ("%[^\n]s" , input);
}

/**
 * @brief scans the standard input for a size for the board, assigns it to the boardsize variable, and returns
 * true orr false if it's valid or not
 * @param boardSize a pointer to the integer that will hold the board size
 * @return 1 if the board size is ok and within limits, 0 otherwise.
 */
int checkBoardSize (int* boardSize)
{
	printf ("enter board size: ");
	if (! scanf ("%d" , boardSize))
	{
		fprintf (stderr , "please enter exactly 1 number");
		return 0;
	}
	if (*boardSize < MIN_BOARD_SIZE)
	{
		fprintf (stderr , "there is no possible position for the ships! the board is too small");
		return 0;
	}
	if (*boardSize > MAX_BOARD_SIZE)
	{
		fprintf (stderr , "board size not fitting!\n");
		return 0;
	}
	return 1;
}

/**
 * @brief  extracts the column number from the user's input (we will enforce that the column number must be the 3rd
 *  character inserted by the user).
 * @param input the string accepted from the user
 * @return the column number
 */
int getColNum (const char* input)
{
	return atoi (&input[2]) - 1;
}
/**
 * @brief checks if the row and column number the user inserted make a legal move on the board
 * @param boardSize the size of the board
 * @param row the row number
 * @param col the column number
 * @return true (1) if the numbers fit in the board, and false (0) otherwise
 */
int inRange (const int boardSize , const int row , const int col)
{
	return (row >= 0 && row < boardSize && col < boardSize && col >= 0);
}
/**
 * @brief receives the user's input and returns if the move is valid or not
 * @param input the input the user game, we assume that the row isa  lowercase a and if the first character,
 * and the the column number is the 3 character and a normal number.
 * @param row a pointer to where we would like to store the row number
 * @param col a pointer to where we would like to store the column number
 * @param boardSize the size of the board
 * @return true(1) if the input from the user is a legal move on the board, and false(0) otherwise
 */
int checkMove (const char* input , int* row , int* col , const int boardSize)
{
	const int lowerCaseA = 'a';
	*col = getColNum (input);
	*row = input[0] - lowerCaseA;
	return inRange (boardSize , *row , *col);
}
/**
 * @brief checks if the user input is not "exit"
 * @param input the user input
 * @returntrue if not "exit" and false if equals exactly "exit"
 */
bool isNotExit (const char* input)
{
	return ! (input[0] == 'e' && input[1] == 'x' && input[2] == 'i' && input[3] == 't');
}


int main ()
{
	//handle the board size input
	int boardSize = 0;
	if (! checkBoardSize (&boardSize))
	{
		return 0;
	}
	printf (INITIAL_MESSAGE"\n");

	//initialize the board and ships
	Ship ships[NUM_SHIPS] = {0};
	int** board = 0; //otherwise throws a warning
	board = (int**) malloc (sizeof (int*) * boardSize);
	initializeBoard (board , boardSize);
	assignMemForShips (ships);
	placeShips (boardSize , ships , board);
	printBoard (board , boardSize);

	//let's do a turn!
	//here, we ASSUME each input will only be <letter space number> and nothing else, or "exit" with no spaces.
	int BUFFER_SIZE = 128;
	char input[32] = {'0'};
	int col = 0;
	int row = 0;
	getInput (BUFFER_SIZE , input);
	while (isNotExit (input))
	{
		//check if the move is legal
		if (! checkMove (input , &row , &col , boardSize))
		{
			printf (ILLEGAL_MOVE);
			getInput (BUFFER_SIZE , input);
			continue;
		}
		//check if we already hit that spot before
		if (board[row][col] == Missed ||
			board[row][col] == SunkShip)
		{
			printf (HIT_ALREADY);
			getInput (BUFFER_SIZE , input);
			continue;
		}
		// mark the hit, update the board, and print the appropriate message
		registerHit (row , col , board , ships);
		printBoard (board , boardSize);
		//check if the game is over
		if (isGameOver (board , ships , boardSize))
		{
			break;
		}
		getInput (BUFFER_SIZE , input);
	}
	//we have now finished the game, tidy up then finish.
	//free the memory we used up
	for (int i = 0 ; i < NUM_SHIPS ; i ++)
	{
		freeShip (ships[i]);
	}
	freeBoard (board , boardSize);
	if (isNotExit (input))
	{
		printf (GAME_OVER);
	}
	return 0;
}





