
 * @brief the program reads from a file (given through the command line as an argument) and builds a heat map
 * according to the file. it then calculates the spread of heat across the board.
 */


#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "calculator.h"
#include "heat_eqn.h"
/**
 * @brief the location of the file's name in the command line
 */
#define FILELOCATION 1
/**
 * @brief the deliminator line
 */
#define DEL "----\n"
/**
 * @brief checks if the line is a deliminator line
 * @param line the line to check
 * @return true if the line is a deliminator line, and false otherwise
 */
bool getDel (char* line)
{

	if (strcmp (line , DEL) != 0)
	{
		return false;
	}
	return true;
}

/**
 * @brief prints a 2d array (grid)
 * @param grid the grid
 * @param n the row numbers
 * @param m the column numbers
 */
void printGrid (double** grid , size_t n , size_t m)
{
	for (size_t i = 0 ; i < n ; i ++)
	{
		for (size_t j = 0 ; j < m ; j ++)
		{
			printf ("%2.4f," , grid[i][j]);
		}
		printf ("\n");
	}
}

/**
 * @brief prints the array and the difference between the previous heat sum to the current one
 * @param rowNum the number of rows in the grid
 * @param colNum the number of columns in the grid
 * @param grid the gird itself
 * @param diff the difference in heatsums
 */
void printDiffAndGrid (size_t rowNum , size_t colNum , double** grid , double diff)
{
	printf ("%lf\n" , diff);
	printGrid (grid , rowNum , colNum);
}


/**
 * @brief adds a heat source point to the list of them
 * @param sources the pointer to the list of heat source points
 * @param numSource the number of source points
 * @param value the value of the point
 * @param row the row number
 * @param col the column number
 */
void addSource (source_point* sources , size_t numSource , double value , int row , int col)
{
	sources[numSource] . x = row;
	sources[numSource] . y = col;
	sources[numSource] . value = value;
}
/**
 * @brief receives a line read from the file, and extracts from it the the row number, column, and value of a new
 * heat source point. The function then adds it to the list of source points and the grid
 * @param line the line read from the file
 * @param part a pointer to the helper variable which we will use to store the parts of the line we read
 * @param rowNum the number of rows in the grid
 * @param colNum the number of columns in the grid
 * @param grid the grid
 * @param sources the list of source points
 * @param numSource the number of source points
 * @param lineSize the size of the line from the file we received
 * @return
 */
bool getSourcePoint (char* line , char* part , size_t rowNum , size_t colNum , double** grid , source_point* sources ,
					 size_t* numSource , size_t lineSize)
{
	/*
	 * first, we'll create a copy of the line so we won't mess up the original line
	 */
	char* lineCopy = (char*) malloc (lineSize);
	strcpy (lineCopy , line);
	/*
	 * get the first part of the line, the row number. we'll separate it from the line and check it's validity
	 */
	part = strtok (lineCopy , " ,");
	int row = atoi (part) , col = 0;
	if (row == 0 && strcmp (part , "0") != 0)
	{
		return false;
	}
	if ((size_t) row >= rowNum)
	{
		return false;
	}
	/*
	 * get the second part of the line, the column number. we'll separate it from the line and check it's validity
	 */
	part = strtok (NULL , " ,");
	col = atoi (part);
	if ((size_t) col == 0 && strcmp (part , "0") != 0)
	{
		return false;
	}
	if ((size_t) col >= colNum)
	{
		return false;
	}
	/*
	 * get the third part of the line, the heat value. we'll separate it from the line and check it's validity
	 */
	part = strtok (NULL , " ,");
	double value = atof (part);
	if (value == 0 && strcmp (part , "0") != 0)
	{
		return false;
	}
	/*
	 * add the point to the grid
	 */
	grid[row][col] = value;
	/*
	 * create a new heat source point
	 */
	addSource (sources , *numSource , value , row , col);
	++ *numSource;
	/*
	 * check that we don't have another part to the line
	 */
	if (strcmp (&part[strlen (part) - 1] , "\n") != 0)
	{
		return false;
	}
	free (lineCopy);
	return true;
}

/**
 * @brief initializes the memory for the grid, and initializes the values to 0
 * @param grid the gird
 * @param rowNum the number of rows
 * @param colNum the number of columns
 * @return 1 if failed in allocating the memory, and 0 otherwise
 */
int initGrid (double** grid , size_t rowNum , size_t colNum)
{
	for (size_t i = 0 ; i < colNum ; i ++)
	{
		grid[i] = (double*) malloc (sizeof (double) * colNum);
		if (grid[i] == NULL)
		{
			fprintf (stderr , "could not allocate memory for the grid");
			return 1;
		}
	}

	for (size_t i = 0 ; i < rowNum ; i ++)
	{
		for (size_t j = 0 ; j < colNum ; j ++)
		{
			grid[i][j] = 0;
		}
	}
	return 0;
}
/**
 * @brief reads a line from the file
 * @param line a pointer that will hold the line we read
 * @param lineSize a pointer that will hold the size of the line
 * @param fr the file
 * @return true if successfully read the line, and false otherwise.
 */
bool readLine (char** line , size_t* lineSize , FILE* fr)
{
	if (getline (line , lineSize , fr) == - 1)
	{
		return false;
	}
	return true;
}
/**
 * @brief reads the first line in the file
 * @param line a pointer which will hold the first line read
 * @param lineSize a pointer that will hold the size of the line read from the file
 * @param fr the file pointer
 * @param part a pointer that will be used to hold the different subparts of the line
 * @param n a pointer to which we will assign the number of rows that the grid has, being read from the file
 * @param m a pointer to which we will assign the number of columns that the grid has, being read from the file
 * @return true if the first line was read successfully, and false otherwise
 */
bool firstLine (char* line , size_t* lineSize , FILE* fr , char* part , size_t* n , size_t* m)
{
	if (! readLine (&line , lineSize , fr))
	{
		return false;
	}
	char* lineCopy = (char*) malloc (*lineSize);
	strcpy (lineCopy , line);
	part = strtok (lineCopy , " ,");
	*n = (size_t) atoi (part);
	//invalid row input
	if (*n == 0 && strcmp (part , "0") != 0)
	{
		return false;
	}
	part = strtok (NULL , " ,");
	*m = (size_t) atoi (part);
	//invalid column input
	if (*m == 0 && strcmp (part , "0") != 0)
	{
		return false;
	}
	// check if we have further input other than what we were looking for
	if (strcmp (&part[strlen (part) - 1] , "\n") != 0)
	{
		return false;
	}
	free (lineCopy);
	free (line);
	return true;
}
/**
 * @brief reads the last part of the file, and assigns the relevant variables
 * @param line a pointer that will hold each of the lines in the last part of the file
 * @param lineSize a pointer that will hold the size of the line
 * @param fr the pointer to the file
 * @param part a helper variable that will hold the different parts of the line we read
 * @param terminate a pointer to the termination value
 * @param n_iter a pointer to the number of iterations to do
 * @param is_cyclic a pointer to the is_cyclic variable
 * @return true if everything was read correctly from the file, and false otherwise
 */
bool lastPart (char* line , size_t* lineSize , FILE* fr , char* part , double* terminate , unsigned int* n_iter ,
			   int* is_cyclic)
{
	if (! readLine (&line , lineSize , fr))
	{
		return false;
	}
	char* lineCopy = (char*) malloc (*lineSize);
	strcpy (lineCopy , line);
	part = strtok (lineCopy , " ");
	*terminate = atof (part);
	//invalid terminate value input
	if (*terminate == 0 && strcmp (part , "0\n") != 0)
	{
		return false;
	}

	if (! readLine (&line , lineSize , fr))
	{
		return false;
	}
	part = strtok (line , " ");
	int temp = atoi (part);
	//invalid terminate value input
	if ((temp == 0 && strcmp (part , "0\n") != 0) || temp < 0)
	{
		return false;
	}
	*n_iter = (unsigned) temp;
	if (! readLine (&line , lineSize , fr))
	{
		return false;
	}
	part = strtok (line , " ");
	*is_cyclic = atoi (part);
	//invalid terminate value input
	switch (*is_cyclic)
	{
		case 0:
			if (strcmp (part , "0\n") != 0)
			{
				return false;
			}
			break;
		case 1:
			break;
		default:
			return false;
	}
	free (lineCopy);
	return true;
}

int main (int argc , char* argv[])
{
	FILE* fr;
	char* line = NULL;
	size_t lineLength = 0 , rowNum = 0 , colNum = 0 , num_source = 0;
	int is_cyclic = 0;
	char* part = NULL;
	double terminate = 0;
	unsigned int n_iter = 0;
	/*
	 * check that we only got 1 parameter in the command line
	 */
	if (argc != 2)
	{
		fprintf (stderr , "please enter exactly one argument");
		return 1;
	}
	/*
	 * make sure the file opens nicely
	 */
	fr = fopen (argv[FILELOCATION] , "r");
	if (fr == NULL)
	{
		fprintf (stderr , "could not open the file");
		return 1;
	}
	/*
	 * read the first line
	 */
	if (! firstLine (line , &lineLength , fr , part , &rowNum , &colNum))
	{
		fprintf (stderr , "error");
		return 1;
	}
	/*
	 * make sure the second line is a deliminator line
	 */
	if (! readLine (&line , &lineLength , fr))
	{
		fprintf (stderr , "error");
		return 1;
	}
	if (! getDel (line))
	{
		fprintf (stderr , "error, second line not a deliminator line");
		return 1;
	}

	/*
	 * assign the grid, set it to zero. assign the source points to be an array of size rowNum*colNum
	 */
	double** grid;
	grid = (double**) malloc (sizeof (double*) * rowNum);
	if (grid == NULL)
	{
		fprintf (stderr , "problem making the grid");
		return 1;
	}
	if (initGrid (grid , rowNum , colNum) == 1)
	{
		fprintf (stderr , "problem making the grid");
		return 1;
	}
	source_point* sources = (source_point*) malloc (sizeof (source_point) * rowNum * colNum);
	if (sources == NULL)
	{
		fprintf (stderr , "error allocating the memory for the source points");
		return 1;
	}

	/*
	 * go over the lines, until either EOF or a deliminator
	 */
	while (readLine (&line , &lineLength , fr))
	{
		if (getDel (line))
		{
			break;
		}
		if (! getSourcePoint (line , part , rowNum , colNum , grid , sources , &num_source , lineLength))
		{
			fprintf (stderr , "bad source point line");
			return 1;
		}
	}
	/*
	 * now, if we got to this point, either we have reached the deliminator line, and there are no more source points.
	 */
	if (! getDel (line))
	{
		fprintf (stderr , "please enter a valid deliminator line (---) after the source points");
		return 1;
	}
	/*
	 * now, finally, read the last 3 lines
	 */
	if (! lastPart (line , &lineLength , fr , part , &terminate , &n_iter , &is_cyclic))
	{
		fprintf (stderr , "please enter 3 numbers, separated by a line to finish the file");
		return 1;
	}

	/*
	 * after all that input, time to calculate!
	 */
	double diff = calculate (heat_eqn , grid , rowNum , colNum , sources , num_source , terminate , n_iter , is_cyclic);
	printDiffAndGrid (rowNum , colNum , grid , diff);
	while (diff > terminate)
	{
		diff = calculate (heat_eqn , grid , rowNum , colNum , sources , num_source , terminate , n_iter , is_cyclic);
		printDiffAndGrid (rowNum , colNum , grid , diff);
	}
	/*
	 * free the memory used
	 */
	free (sources);
	for (size_t i = 0 ; i < rowNum ; i ++)
	{
		free (grid[i]);
	}
	free (grid);
	fclose (fr);
	free (line);
	return 0;
}


