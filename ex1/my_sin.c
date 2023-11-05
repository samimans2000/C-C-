#include <stdio.h>
#include <stdlib.h>

#define PI 3.141529

/** \brief calculates the value of sin(x) by calculating recursively
* if x<0.0.1, then return x, otherwise, return 3*sin(x/3)-4*(sin(x/3)^3)
* \param x double the value to calculate sins(x) at
* \return double sin(x)
*
*/
double calSin(double x)
{
    if (x < 0.01 && x > - 0.01)
    {
        return x;
    }
    x = calSin(x / 3);
    return 3 * x - 4 * x * x * x;
}


/** \brief receives a number from the user, and calculates sin(x)
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
    printf("%lf", calSin(x));
    return 0;
}
