#include "main.ih"
/*
 * 
 * 
 */ 

// size_t exportBaseline(vector<double> xpos, vector<double> ypos, vector<double> psize, string subID, string trialNum, ofstream& outputfile)
void exportBaseline(vector<double> psize, string subID, string trialNum, ofstream& outputfile)
{
  // make average baseline and write result to file
  outputfile << subID << '\t' 
		<< trialNum << '\t'
//     << vectorMean(xpos) << '\t' 
//     << vectorMean(ypos) << '\t' 
    << vectorMean(psize) << '\n';

//   return 0;
}