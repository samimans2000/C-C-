
 * @brief calculates the spread of heat across a 2D grid, to within a certain precision level.
 */

#include "calculator.h"
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
/**
 * @brief returns the left index to the current column in a cyclic manner in a 2d array.
 */
#define CYCLIC_LEFT_SAFE(numCol , currCol) (currCol - 1 + numCol) % numCol
/**
 * @brief returns the right index to the current column in a cyclic manner in a 2d array.
 */
#define CYCLIC_RIGHT_SAFE(numCol , currCol) (currCol + 1 + numCol) % numCol
/**
 * @brief returns the up index to the current row in a cyclic manner in a 2d array.
 */
#define CYCLIC_UP_SAFE(numRow , currRow) (currRow - 1 + numRow) % numRow
/**
 * @brief returns the down index to the current row in a cyclic manner in a 2d array.
 */
#define CYCLIC_DOWN_SAFE(numRow , currRow) (currRow + 1 + numRow) % numRow

enum Direction
{
	Up ,
	Down ,
	Left ,
	Right
};

/**
 * @brief receives a row and column number, and the list of source points, and checks whether the point indicated by
 * the row and the col numbers is a source point
 * @param sources a pointer to the list of source points
 * @param num_sources the number of source points
 * @param row the row number to check
 * @param col the column number to check
 * @return true if the row and col represent a source point, and false otherwise
 */
bool isSourcePoint (source_point* sources , size_t num_sources , size_t row , size_t col)
{
	size_t i = 0;
	for (i = 0 ; i < num_sources ; i ++)
	{
		if (sources[i] . x == (int) row && sources[i] . y == (int) col)
		{
			return true;
		}
	}
	return false;
}

/**
 * @brief returns the value of a 2d array in the direction "dir" from a current location.
 * the function can return a cyclic value or a 0, if the direction exceeds the array limits.
 * @param n the number of rows in the 2d array
 * @param m the number of columns in the 2d array
 * @param currRow the current row of the cell we wish to look at it's neighbor
 * @param currCol the current column of the cell we wish to look at it's neighbor
 * @param is_cyclic a boolean value which indicates whether the value of off limit place in the grid will be zero, or
 * wrap around the array
 * @param grid the grid from which we want to retrieve the values
 * @param dir the direction from the cell which we want to get it's value
 * @return
 */
double safeReturnValue (size_t n , size_t m , size_t currRow , size_t currCol , int is_cyclic , double** grid ,
						enum Direction dir)
{
	switch (dir)
	{
		case Up:
			if (is_cyclic)
			{
				return grid[CYCLIC_UP_SAFE (n , currRow)][currCol];
			}
			else
			{
				return ((int) currRow - 1 < 0) ? 0 : grid[currRow - 1][currCol];
			}
		case Down:
			if (is_cyclic)
			{
				return grid[CYCLIC_DOWN_SAFE (n , currRow)][currCol];
			}
			else
			{
				return (currRow + 1 >= n) ? 0 : grid[currRow + 1][currCol];
			}
		case Left:
			if (is_cyclic)
			{
				return grid[currRow][CYCLIC_LEFT_SAFE (m , currCol)];
			}
			else
			{
				return ((int) currCol - 1 < 0) ? 0 : grid[currRow][currCol - 1];
			}
		default: //ergo - right
			if (is_cyclic)
			{
				return grid[currRow][CYCLIC_RIGHT_SAFE (m , currCol)];
			}
			else
			{
				return (currCol + 1 >= m) ? 0 : grid[currRow][currCol + 1];
			}
	}
}

/**
 * @brief applies the diff function to each non source cell in the grid
 * @param function the heat function to apply
 * @param grid the gird of points (2d array)
 * @param n the row number of the grid
 * @param m the col number of the grid
 * @param sources a pointer to the heat source points
 * @param num_sources the number of source points
 * @param is_cyclic a boolean value to indicate whether the neighbors of each cell are cyclic or strict 0's.
 */
void applyHeatFunction (diff_func function , double** grid , size_t n , size_t m , source_point* sources ,
						size_t num_sources , int is_cyclic)
{
	size_t row = 0 , col = 0;
	for (row = 0 ; row < n ; row ++)
	{
		for (col = 0 ; col < m ; col ++)
		{
			if (! isSourcePoint (sources , num_sources , row , col))
			{
				grid[row][col] = function (grid[row][col] ,
										   safeReturnValue (n , m , row , col , is_cyclic , grid , Right) ,
										   safeReturnValue (n , m , row , col , is_cyclic , grid , Up) ,
										   safeReturnValue (n , m , row , col , is_cyclic , grid , Left) ,
										   safeReturnValue (n , m , row , col , is_cyclic , grid , Down));
			}
		}
	}
}

/**
 * @brief sums all of the values of a 2d grid
 * @param grid the grid
 * @param n the row number
 * @param m the column number
 * @return the sum of all the values of the grid
 */
double sumGrid (double** grid , size_t n , size_t m)
{
	double sum = 0;
	for (size_t row = 0 ; row < n ; row ++)
	{
		for (size_t col = 0 ; col < m ; col ++)
		{
			sum += grid[row][col];
		}
	}
	return sum;
}

/**
 * @brief updates the values of the sums of the grid, and applies the heat function to the grid
 * @param function the heat function to be applied to the grid
 * @param grid the gird
 * @param n the number of rows
 * @param m the number of columns
 * @param sources a pointer to the source points
 * @param num_sources the number of source points
 * @param is_cyclic a boolean value to indicate whether the neighbors of each cell are cyclic or strict 0's.
 * @param prevSum the previous sum of the grid, before we apply the heat function
 * @param currSum the current sum of the grid, after applying the heat function
 */
void updateGridAndSums (diff_func function , double** grid , size_t n , size_t m , source_point* sources ,
						size_t num_sources , int is_cyclic , double* prevSum , double* currSum)
{
	*prevSum = *currSum;
	applyHeatFunction (function , grid , n , m , sources , num_sources , is_cyclic);
	*currSum = sumGrid (grid , n , m);
}

/**
 * Calculator function. Applies the given function to every point in the grid iteratively for n_iter loops, or until the cumulative difference is below terminate (if n_iter is 0).
 */
double calculate (diff_func function , double** grid , size_t n , size_t m , source_point* sources ,
				  size_t num_sources , double terminate , unsigned int n_iter , int is_cyclic)
{
	/*
	 * calculate the initial heat value of the map
	 */
	double prevSumHeat = sumGrid (grid , n , m);
	double currSumHeat = prevSumHeat;
	/*
	 * if n_iter is greater than 0, we'll run the function n times, and return the difference
	 */
	if (n_iter != 0)
	{
		for (size_t i = 0 ; i < n_iter ; i ++)
		{
			updateGridAndSums (function , grid , n , m , sources , num_sources , is_cyclic , &prevSumHeat ,
							   &currSumHeat);
		}
		return fabs (currSumHeat - prevSumHeat);
	}
	/*
	 * if we reached here, n_iter is 0, and we will run the function until the delta is smaller than the difference
	 */
	assert (n_iter == 0);
	updateGridAndSums (function , grid , n , m , sources , num_sources , is_cyclic , &prevSumHeat , &currSumHeat);
	double delta = fabs (currSumHeat - prevSumHeat);
	while (delta > terminate)
	{
		updateGridAndSums (function , grid , n , m , sources , num_sources , is_cyclic , &prevSumHeat , &currSumHeat);
		delta = fabs (currSumHeat - prevSumHeat);
	}
	return delta;
}
