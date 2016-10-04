#include "main.ih"

double vectorMedian(vector<double> dataArray) 
{
  
  size_t iSize = dataArray.size();

  // sort array
  for (size_t elArray1 = iSize - 1; elArray1 > 0; --elArray1) 
  {
    for (size_t elArray2 = 0; elArray2 < elArray1; ++elArray2) 
    {
      if (dataArray[elArray2] > dataArray[elArray2 + 1]) 
      {
	double dTemp = dataArray[elArray2];
	dataArray[elArray2] = dataArray[elArray2 + 1];
	dataArray[elArray2 + 1] = dTemp;
       }
     }
   }
   
   // Middle or average of middle values in the sorted array.
  double dMedian = 0.0;
  if ((iSize % 2) == 0) 
    dMedian = (dataArray[iSize/2] + dataArray[(iSize/2) - 1])/2.0;
  else
    dMedian = dataArray[iSize/2];
  
  vector<double>().swap(dataArray);
  return dMedian;
}