//
// Created by noamfi on 8/9/18.
//
// ------------------------------ includes ------------------------------
#ifndef EX2_BATTLESHIPS_H
#define EX2_BATTLESHIPS_H

#endif //EX2_BATTLESHIPS_H

#include <stdbool.h>
// ------------------------------ const definitions ------------------------------
/**
 * @def NUM_SHIPS 5
 * @brief the number of ships in the game
 */
#define NUM_SHIPS 5
/**
 * @def CARRIER_LENGTH 5
 * @brief the length of the aircraft carrier
 */
#define CARRIER_LENGTH 5
/**
 * @def PATROL_BOAT_LENGTH 4
 * @brief the length of the patrol boat
 */
#define PATROL_BOAT_LENGTH 4
/**
 * @def MISSILE_BOAT_LENGTH 3
 * @brief the length of the missile boat
 */
#define MISSILE_BOAT_LENGTH 3
/**
 * @def SUBMARINE_LENGTH 3
 * @brief the length of the submarine
 */
#define SUBMARINE_LENGTH 3
/**
 * @def DESTROYER_LENGTH 2
 * @brief the length of the destroyer
 */
#define DESTROYER_LENGTH 2
/**
 * @def HIT "Hit.\n"
 * @brief the hit message
 */
#define HIT "Hit.\n"
/**
 * @def HIT_AND_SUNK "Hit and sunk.\n"
 * @brief the hit and sunk message
 */
#define HIT_AND_SUNK "Hit and sunk.\n"
/**
 * @def MISS "Miss.\n"
 * @brief the miss message
 */
#define MISS "Miss.\n"
/**
 * @def GAME_OVER "Game over"
 * @brief the game over message
 */
#define GAME_OVER "Game over"
/**
 * @def ILLEGAL_MOVE "Invalid move, try again.\n"
 * @brief the illegal move message
 */
#define ILLEGAL_MOVE "Invalid move, try again.\n"
/**
 * @def INITIAL_MESSAGE "Ready to play."
 * @brief the ready to play message
 */
#define INITIAL_MESSAGE "Ready to play."
/**
 * @def HIT_ALREADY "Already been Hit.\n"
 * @brief the already hit message
 */
#define HIT_ALREADY "Already been Hit.\n"

// ------------------------------ structs and functions ------------------------------
struct Coordinate;
struct BattleShip;
typedef struct BattleShip* Ship;
enum Status
{
	Missed = - 1 ,
	Nada ,
	SunkShip ,
	HiddenShip
};

/**
 * @brief marks the hit entered by the user on to the board and checks if the bomb hit a ship. In the case that it did,
 * the function checks whether the ship has sunk. The function then prints the message to the user
 * @param row the row number of the hit
 * @param col the col number of the hit
 * @param board the pointer to the board
 * @param ships an array of all of the ships in the game
 */
void registerHit (int row , int col , int** board , Ship ships[NUM_SHIPS]);
/**
 * @brief checks whether the game is over.
 * @param board the pointer to the board
 * @param ships an array of the ships in the game
 * @param boardSize the size of the board
 * @return true if the game is over (meaning, if all the ships have been sunk or the board is full) and false otherwise.
 */
bool isGameOver (int** board , Ship ships[NUM_SHIPS] , int boardSize);

/**
 * @brief places the ships on the board, and updates the board and the ship's location accordingly
 * @param boardSize the size of the board
 * @param shipSize the size of the ship
 * @param board the pointer to the board
 * @param shipsCoordinate an array of the size of the size, where each cell holds the place on the board where it's at
 */
void assignSpace (const int boardSize , const int shipSize , int** board , struct Coordinate* shipsCoordinate);

/**
 * @brief  initializes all of a ship's "fields" ("constructor"-like)
 * @param ship a pointer to a struct of type BattleShip, which we wish to initialize
 * @param shipSize the size of the ship
 * @param board a pointer to the board
 * @param boardSize the size of the board
 */
void initializeShip (Ship ship , const int shipSize , int** board , const int boardSize);

/**
 * @brief initializes the memory for the board according to the size inputted by user, and sets the board to blank
 * @param board the pointer to the board we wish to initialize
 * @param boardSize the size of the board
 */
void initializeBoard (int** board , int boardSize);

/**
 * @brief a helper function that prints a memory error message and quits the function
 */
void exitNoMemory ();

/**
 * @brief frees all of the memory that was allocated to a ship and it's fields.
 * @param ship the ship we wish to free it's memory
 */
void freeShip (Ship ship);

/**
 * @brief frees all of the memory of the board
 * @param board the pointer to the board we wish to free
 * @param boardSize the size of the board
 */
void freeBoard (int** board , int boardSize);

/**
 * @brief prints the board
 * @param board a pointer to board
 * @param boardSize the board size
 */
void printBoard (int** board , int boardSize);

/**
 * @brief assign the memory for the ships of the game
 * @param ships the array containing all of the ships in the game
 */
void assignMemForShips (Ship* ships);

/**
 * @brief calls the assignSpace function for each of the ships in the array ships
 * @param boardSize the size of the board
 * @param ships the array of ships in the game
 * @param board a pointer to the board
 */
void placeShips (int boardSize , Ship* ships , int** board);
/**
 * @brief checks if the board is full (meaning all of the spots have been hit)
 * @param board a pointer to the board
 * @param boardSize the size of the board
 * @return true if the board is used up, and false otherwise
 */
bool isBoardFull (int** board , int boardSize);
/**
 * @brief checks if all of the boats in the game have been sunk
 * @param ships an array to all of the ships in the game
 * @return true if all of the ships have been sunk, and false otherwise
 */
bool isAllSunk (Ship ships[NUM_SHIPS]);
