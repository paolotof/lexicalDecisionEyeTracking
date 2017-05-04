#include "main.ih"

size_t binData(string filename, size_t lines2bin)
{
// first determine whether it is eye movements or pupil size
	bool ERD = false;
	if (filename.find("Baseline") != string::npos)
		ERD = true;
	
	cout << ERD << '\n';
	
  ifstream datafile(filename);
  if (! datafile.is_open())
  {
    cout << "datafile to bin does not exists\n";
    return (0);
  }
  cout << "BINNING\n";
	ofstream binnedData;
  filename.insert(filename.find(".asc"), "_binned");

  binnedData.open(filename);//, std::ios::app); 
  if (! binnedData.is_open())
  {
    cout << "binned file does not exists\n";
    return (0);
  }
// write header  
	binnedData << "pp" << '\t' << "time" << '\t' 
		<< "wordNonWord" << '\t'  << "trial"  << '\t'
		<< "item"  << '\t';
	if (ERD)
		binnedData << "baseline1" << '\t' << "baseline2"; 
	else
		binnedData << "psize";
	binnedData << '\t' << "fix\n";
		
  
  vector<double> timeV; // time
  vector<double> fixV;  // fixations
//   if (not ERD)
		vector<double> psizeV; // psize
// 	else	
// 	{
		vector<double> baseOne;
		vector<double> baseTwo;
// 	}
  
  string previousPp = "";
  size_t previousTr = 1;
  string dataline;
	// read first line and write header file
// 	getline(datafile, dataline);
// 	binnedData << dataline << '\n';
  while (getline(datafile, dataline))
  {
    istringstream linedata(dataline); // extract time information from the eyetracking file
    string currentPp, item;
    size_t clockTime, condition, currentTr;
    double psize, time, fix, b1, b2;//, RT, acc
    if (not ERD)
			linedata >> currentPp >> clockTime >> 
				time >> condition >> currentTr >> item >> 
				psize >> 
				fix;// >> RT >> acc;
		else
			linedata >> currentPp >> 
				time >> condition >> currentTr >> item >> 
				b1 >> b2 >> 
				fix;// >> RT >> acc;
		
    // reset everything  
    if ((previousPp != currentPp) || (previousTr != currentTr))
    {
      vector<double>().swap(timeV);
      vector<double>().swap(fixV);
			if (not ERD)
				vector<double>().swap(psizeV);
			else
			{
				vector<double>().swap(baseOne);
				vector<double>().swap(baseTwo);
			}
    }
    // this must be done afterwards otherwise all the values of the first timepoint
    // are flushed instead of stored. 
    timeV.push_back(time);
		fixV.push_back(fix);
		if (not ERD)
			psizeV.push_back(psize);
		else
		{
			baseOne.push_back(b1);
			baseTwo.push_back(b2);
		}
    // write mean out
    if (timeV.size() == lines2bin)
    {
			if (not ERD)
			{
				binnedData << currentPp << '\t'
				// I don't think this is necessary since we are only using the time and not the 'system time'
	// 			size_t old_precision = binnedData.precision();
	// 			binnedData.precision(10);
	// 			binnedData << vectorMean(timeV) << '\t';
	// 			binnedDat.precision(old_precision);
	// 			binnedData 
					<< vectorMean(timeV) << '\t'
					<< condition << '\t' 
					<< currentTr  << '\t' << item << '\t' 
					<< vectorMean(psizeV) << '\t' << vectorMean(fixV) << '\t' //<< RT << '\t' 
					//<< acc << '\t' 
					<< '\n';
				vector<double>().swap(timeV);
				vector<double>().swap(psizeV);
				vector<double>().swap(fixV);
			} else {
				binnedData << currentPp << '\t'
				<< vectorMean(timeV) << '\t'
				<< condition << '\t' 
				<< currentTr  << '\t' << item << '\t' 
				<< vectorMean(baseOne) << '\t' << vectorMean(baseTwo) << '\t' 
				<< vectorMean(fixV) << '\t'
				<< '\n';
				vector<double>().swap(timeV);
				vector<double>().swap(baseOne);
				vector<double>().swap(baseTwo);
				vector<double>().swap(fixV);
			}
			
    }
    
    previousPp = currentPp;
    previousTr = currentTr;
          
  } // end "while(getline(trialInfoFile, trialInfo))"
  
  datafile.close();
  binnedData.close();
  
  return 0;
}
