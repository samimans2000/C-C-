
* @section DESCRIPTION
* The program handles the game logic and the "behind the scenes"
* Input  : the messages to be printed to the user
*/
// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "battleships.h"

// ------------------------------  structs and functions ------------------------------
enum Direction
{
	Up ,
	Down
};
/**
 * Represents a Coordinate. Each ship has the amount of coordinates corresponding to it's size, holding where
 * it is on the board.
 */
typedef struct Coordinate
{
	int x;
	int y;
} Coordinate;
/**
 * A type representing a battleship. Each battle ship holds an array of coordinates of it's size, with each one holding
 * the location of that part of the ship n the board. In addition, each ship holds information on whether it was sunk,
 * and it' size.
 */
typedef struct BattleShip
{
	Coordinate* displacement;
	int size;
	int numTimesHit;
	bool sunk;
} BattleShip;

typedef BattleShip* Ship;

void assignSpace (const int boardSize , int shipSize , int** board , Coordinate* shipsCoordinate)
{
	int i = 0 , row , col;
	bool emptySpace = true;
	enum Direction direction;
	do
	{
		direction = rand () % 2;
		row = rand () % boardSize;
		col = rand () % boardSize;
		for (i = 0 ; i < shipSize ; i ++)
		{
			if (direction == Up)
			{
				emptySpace = (row + i < boardSize) && (board[row + i][col] == 0);
			}
			else
			{
				emptySpace = (col + i < boardSize) && (board[row][col + i] == 0);
			}
			if (! emptySpace)
			{
				break;
			}
		}
	} while (! emptySpace);
	for (i = 0 ; i < shipSize ; i ++)
	{
		if (direction == Up)
		{
			shipsCoordinate[i] . x = row + i;
			shipsCoordinate[i] . y = col;
			board[row + i][col] = HiddenShip;
		}
		else
		{
			shipsCoordinate[i] . x = row;
			shipsCoordinate[i] . y = col + i;
			board[row][col + i] = HiddenShip;
		}
	}
}

void registerHit (int row , int col , int** board , Ship ships[NUM_SHIPS])
{
	//update the board
	board[row][col] --;
	//now, we'll check if we hit a ship
	for (int i = 0 ; i < NUM_SHIPS ; i ++)
	{
		for (int j = 0 ; j < ships[i] -> size ; j ++)
		{
			if (ships[i] -> displacement[j] . x == row && ships[i] -> displacement[j] . y == col)
			{
				ships[i] -> numTimesHit ++;
				if (ships[i] -> numTimesHit == ships[i] -> size)
				{
					ships[i] -> sunk = true;
					printf (HIT_AND_SUNK);
					return;
				}
				printf (HIT);
				return;
			}
		}
	}
	printf (MISS);
}

bool isAllSunk (Ship ships[NUM_SHIPS])
{
	for (int i = 0 ; i < NUM_SHIPS ; i ++)
	{
		if (! ships[i] -> sunk)
		{
			return false;
		}
	}
	return true;
}

bool isBoardFull (int** board , int boardSize)
{
	for (int i = 0 ; i < boardSize ; i ++)
	{
		for (int j = 0 ; j < boardSize ; j ++)
		{
			if (board[i][j] == Nada || board[i][j] == HiddenShip)
			{
				return false;
			}
		}
	}
	return true;
}

bool isGameOver (int** board , Ship ships[NUM_SHIPS] , int boardSize)
{
	//first, we'll check if all the ships have been sunk.
	bool isOver = isAllSunk (ships);
	if (isOver)
	{
		return true;
	}
	isOver = isBoardFull (board , boardSize);
	return isOver;
}


void assignMemForShips (Ship* ships)
{
	for (int i = 0 ; i < NUM_SHIPS ; i ++)
	{
		ships[i] = (Ship) malloc (sizeof (Ship) + 2 * sizeof (enum Direction) + 1);
		if (ships[i] == NULL)
		{
			exitNoMemory ();
		}
	}
}

void initializeShip (Ship ship , const int shipSize , int** board , const int boardSize)
{
	//first, we'll assign the memory needed for the ship
	assert (ship != NULL);
	ship -> sunk = false;
	ship -> size = shipSize;
	ship -> numTimesHit = 0;
	ship -> displacement = (Coordinate*) malloc (sizeof (Coordinate) * shipSize);
	if (ship -> displacement == NULL)
	{
		exitNoMemory ();
	}
	int i = 0;
	for (i = 0 ; i < shipSize ; i ++)
	{
		ship -> displacement[i] . x = 0;
		ship -> displacement[i] . y = 0;
	}

	// next, we'll randomly find an open space for the ship and place it on the board
	srand ((unsigned int) time (0));
	assignSpace (boardSize , shipSize , board , ship -> displacement);
}

void initializeBoard (int** board , int boardSize)
{
	if (board == NULL)
	{
		exitNoMemory ();
	}
	for (int i = 0 ; i < boardSize ; i ++)
	{
		board[i] = (int*) malloc (sizeof (int) * boardSize);
		if (board[i] == NULL)
		{
			exitNoMemory ();
		}
	}
	for (int i = 0 ; i < boardSize ; i ++)
	{
		for (int j = 0 ; j < boardSize ; j ++)
		{
			board[i][j] = 0;
		}
	}
}


void exitNoMemory ()
{
	fprintf (stderr , "not enough memory, sorry!");
	exit (1);
}

void freeShip (Ship ship)
{
	free (ship -> displacement);
	free (ship);
}

void freeBoard (int** board , int boardSize)
{
	for (int i = 0 ; i < boardSize ; i ++)
	{
		free (board[i]);
	}
	free (board);
}

void printBoard (int** board , int boardSize)
{
	for (int k = - 1 ; k < boardSize ; k ++)
	{
		if (k >= 0)
		{
			printf (",%d" , k + 1);
		}
		else
		{
			printf (" ");
		}
	}
	printf ("\n");
	for (int i = 0 ; i < boardSize ; i ++)
	{
		char lowerCaseA = 'a';
		printf ("%c " , lowerCaseA + i);
		for (int j = 0 ; j < boardSize ; j ++)
		{
			switch (board[i][j])
			{
				case Missed:
					printf ("o");
					break;
				case SunkShip:
					printf ("x");
					break;
				default:
					printf ("_");
			}
			printf (" ");
		}
		printf ("\n");
	}
}

void placeShips (int boardSize , Ship* ships , int** board)
{
	initializeShip (ships[0] , CARRIER_LENGTH , board , boardSize); // the carrier
	initializeShip (ships[1] , PATROL_BOAT_LENGTH , board , boardSize); // the patrol boat
	initializeShip (ships[2] , MISSILE_BOAT_LENGTH , board , boardSize); // the missile boat
	initializeShip (ships[3] , SUBMARINE_LENGTH , board , boardSize); // the submarine
	initializeShip (ships[4] , DESTROYER_LENGTH , board , boardSize); // (seriously? in rl, it's a huge ship!)
}
