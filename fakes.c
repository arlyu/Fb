#include "fakes.h"
#include "math.h"

#define SQRT2 1.41421356
#define INVSQRT2 0.70710678
#define PI 3.14159265

static double aprcos(double x)
{
    return 1-x*x/2;
}

double mod(double x, double n)
{
    return (x - n * floor(x / n));
}

double fcos(double x)
{
    double res = 0u;

    x = 2*x*SQRT2/PI;   // Fixing period
    double h1 = mod(x * INVSQRT2, 2) - mod(x * INVSQRT2, 1);
    double h0 = 1 - h1;
    double h2 = 1 - 2 * (mod(x * INVSQRT2/2, 2) - mod(x * INVSQRT2/2, 1));

    res = h2 * (aprcos(mod(x,SQRT2))*h0-aprcos(mod(-x,SQRT2))*h1);

    return res;
}

double fsin(double x)
{
    return fcos(x-PI/2);
}
