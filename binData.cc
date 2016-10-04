#include "main.ih"

size_t binData(string filename, size_t lines2bin)
{
  string tmp = filename;
  tmp.append("_ALL_medianInterp.asc");
  ifstream datafile(tmp);
  if (! datafile.is_open())
  {
    cout << "datafile to bin does not exists\n";
    return (0);
  }
  cout << "BINNING\n";
	ofstream binnedData;
  tmp.insert(tmp.find(".asc"), "_binned");

  binnedData.open(tmp);//, std::ios::app); 
  if (! binnedData.is_open())
  {
    cout << "binned file does not exists\n";
    return (0);
  }
  vector<double> timeV; // time
  vector<double> binV; // bin
  vector<double> psizeV; // psize
  vector<double> fixV;  // fixations
  
//   size_t lines2bin = 5;
  string previousPp = "";
  size_t previousTr = 1;
  string dataline;
	// write header file
	binnedData << getline(datafile, dataline);
  while (getline(datafile, dataline))
  {
    istringstream linedata(dataline); // extract time information from the eyetracking file
    string currentPp, item;
    size_t clockTime, condition, currentTr;
    int bin; // time can also be negative, but it will always be an integer
    double psize, time, fix;//, RT, acc
    linedata >> currentPp >> clockTime >> time >> bin >> condition >> currentTr >> 
      item >> psize >> fix;// >> RT >> acc;
    // reset everything  
    if ((previousPp != currentPp) || (previousTr != currentTr))
    {
      vector<double>().swap(timeV);
      vector<double>().swap(binV);
      vector<double>().swap(psizeV);
      vector<double>().swap(fixV);
    }
    // this must be done afterwards otherwise all the values of the first timepoint
    // are flushed instead of stored. 
    timeV.push_back(time);
    binV.push_back(bin);
    psizeV.push_back(psize);
    fixV.push_back(fix);
    // write mean out
    if (timeV.size() == lines2bin)
    {
      binnedData << currentPp << '\t'
      // I don't think this is necessary since we are only using the time and not the 'system time'
// 			size_t old_precision = binnedData.precision();
// 			binnedData.precision(10);
// 			binnedData << vectorMean(timeV) << '\t';
// 			binnedDat.precision(old_precision);
// 			binnedData 
				<< vectorMean(timeV) << '\t'
				<< vectorMean(binV) << '\t' << condition << '\t' 
				<< currentTr  << '\t' << item << '\t' << vectorMean(psizeV) << '\t' 
				<< vectorMean(fixV) << '\t' //<< RT << '\t' 
				//<< acc << '\t' 
				<< vectorMean(fixV) << '\n';
      vector<double>().swap(timeV);
      vector<double>().swap(binV);
      vector<double>().swap(psizeV);
      vector<double>().swap(fixV);
    }
    
    previousPp = currentPp;
    previousTr = currentTr;
          
  } // end "while(getline(trialInfoFile, trialInfo))"
  
  datafile.close();
  binnedData.close();
  
  return 0;
}