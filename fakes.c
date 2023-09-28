#include "fakes.h"
#include "math.h"

#define SQRT2 1.41421356
#define INVSQRT2 0.70710678
#define PI 3.14159265
#define ROOT4 1.59245043

static double aprcos(double x)
{
    return 1-x*x/2;
}

static double aprcos4(double x)
{
    register double res = x*x;
    return 1-(res/2)+(res*res/24);
}

double mod(double x, double n)
{
    return (x - n * floor(x / n));
}

double fcos(double x)
{
    double res = 0u;

    x = 2*x/PI;   // Fixing period (SQRT2 is ignored here to optimize calculus)
    double h1 = mod(x, 2) - mod(x, 1);
    double h0 = 1 - h1;
    double h2 = 1 - 2 * (mod(x/2, 2) - mod(x/2, 1));

    res = h2 * (aprcos(mod(x*SQRT2,SQRT2))*h0-aprcos(mod(-x*SQRT2,SQRT2))*h1);

    return res;
}

double fcos4(double x)
{
    double res = 0u;

    x = 2*x/PI;   // Fixing period (ROOT4 is ignored here to optimize calculus)
    double h1 = mod(x, 2) - mod(x, 1);
    double h0 = 1 - h1;
    double h2 = 1 - 2 * (mod(x/2, 2) - mod(x/2, 1));

    res = h2 * (aprcos(mod(x*ROOT4,ROOT4))*h0-aprcos(mod(-x*ROOT4,ROOT4))*h1);

    return res;
}

double fsin(double x)
{
    return fcos(x-PI/2);
}

double fsin4(double x)
{
    return fcos4(x-PI/2);
}
