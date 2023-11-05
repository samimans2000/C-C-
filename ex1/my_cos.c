#include <stdio.h>
#include <stdlib.h>

#define PI 3.141529

/*
 * my_cos.c
 *
 *  Created on: Aug 1, 2018
 *      Author: noamf
 */

/** \brief calculates the value of cos(x) by calculating sin(pi+ x) recursively
 *
 * \param x double the value to calculate cos(x) at
 * \return double cos(x)
 *
 */
double calSin(double x)
{
    if (x < 0.01 && x > - 0.01)
    {
        return x;
    }
    x = calSin(x / 3.0);
    return 3 * x - 4 * x * x * x;
}

/** \brief receives a number from the user, and calculates cos(x)
 *
 * \return int 0, just to make sure everything is fine
 *
 */
int main()
{
    //we'll check that indeed the input from the user is a number, if not, we'll exit
    double x = 0;
    if (scanf("%lf", &x) != 1)
    {
        fprintf(stderr, "not a valid number");
        return 1;
    }
    printf("%lf", calSin(PI / 2 + x));
    return 0;
}