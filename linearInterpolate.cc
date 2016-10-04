#include "main.ih"

double linearInterpolate(double y1, double y2, double mu)
{
   return(y1 * (1-mu) + y2*mu);
}
