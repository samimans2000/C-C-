//
// Created by noamf on 31-Jul-18.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE  128
#define LETTERS 26


/** \brief returns the letters index number (for instance, a is 0, b=1 ect)
 *
 * \param letter char the letter we wish to look for
 * \param letters[26] const char an array containing the english alphabet. Can be either
 * lowercase or uppercase.
 * \return int the index of the letter, and 26 if the character isn't a letter
 *
 */
int lettersNumber (char letter , const char letters[26])
{
	int i;
	for (i = 0 ; i < LETTERS ; i ++)
	{
		if (letters[i] == letter)
		{
			break;
		}
	}
	return i;
}

/** \brief "encrypts" a single letter.
 *
 * \param letter char the letter to be encrypted
 * \param key long the key of the encryption
 * \return char the encrypted letter (the letter moved to the right by key). if the input
 * character isn't a letter, than we just return the character with no changes.
 *
 */
char changeLetter (char letter , long key)
{
	char lowerCase[LETTERS] = 
	{
			'a' , 'b' , 'c' , 'd' , 'e' , 'f' , 'g' , 'h' , 'i' , 'j' , 'k' , 'l' ,
			'm' , 'n' , 'o' , 'p' ,
			'q' ,
			'r' , 's' , 't' , 'u' , 'v' , 'w' , 'x' , 'y' , 'z'
	};
	char upperCase[LETTERS] = 
	{
			'A' , 'B' , 'C' , 'D' , 'E' , 'F' , 'G' , 'H' , 'I' , 'J' , 'K' , 'L' ,
			'M' , 'N' , 'O' , 'P' ,
			'Q' ,
			'R' , 'S' , 'T' , 'U' , 'V' , 'W' , 'X' , 'Y' , 'Z'
	};
	/**< first, we'll check if the character is a lowercase or uppercase letter, by
	searching for it in both the lowercase and uppercase alphabets.*/
	int indexLowerCase = lettersNumber (letter , lowerCase);
	int indexUpperCase = lettersNumber (letter , upperCase);
	/**< now, we'll encrypt the letter according to the correct case. */
	if (indexLowerCase < 26)
	{
		return lowerCase[(indexLowerCase + key + LETTERS) % LETTERS];
	}
	if (indexUpperCase < 26)
	{
		return upperCase[(indexUpperCase + key + LETTERS) % LETTERS];
	} 
	else
	{
		return letter;
	}
}

/** \brief receives a buffer of letters, encrypts them and prints
 *
 * \param size long the amount of letters in the buffer
 * \param key long the key to be encrypted with
 * \param buffer[] char the buffer filled with letters
 * \return void
 *
 */
void encryptAndPrint (long key , char buffer[] , int size)
{
	int i;
	for (i = 0 ; i < size ; i ++)
	{
		printf ("%c" , changeLetter (buffer[i] , key));
	}
}

/** \brief receives a key, and then a set of characters. Encrypts the letters in them,
*and leaves the rest as is
 *
 * \return int 0 for all good
 *
 */
int main ()
{
	char tempKey;
	long key = 0;
	int size = 0;
	char buffer[BUFFER_SIZE];
	scanf ("%s\n" , &tempKey);
	key = strtol (&tempKey , NULL , 0); // converts the character we received as input to integer
	if (key == 0L && strcmp (&tempKey , "0") != 0)
	{
		fprintf (stderr , "key is not a number");
		return 1;
	}
	if (key > 25 || key < - 25)
	{
		fprintf (stderr , "key is not between -25 and 25");
		return 1;
	}
	while (scanf ("%127c%n" , buffer , &size) != EOF)
	{
		encryptAndPrint (key , buffer , size);
	}
	return 0;
}
