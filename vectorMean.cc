#include "main.ih"

double vectorMean(vector<double> values)
{
  size_t i;
  double sum;
  
  for(i = 0, sum = 0; i < values.size(); sum += values[i++]);
  
  return (sum / values.size());
}