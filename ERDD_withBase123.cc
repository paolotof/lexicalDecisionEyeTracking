#include "main.ih"

// void ERDD_withBase123(string& outputfileName, size_t timeBefore)
void ERDD_withBase123(string& filePrefix, string& outputfileName, size_t timeBefore)
{
	string file;
// 	file += string("RT_") + std::to_string(timeBefore) + "_ALL_medianInterp.asc";
	file += filePrefix + std::to_string(timeBefore) + "_ALL_medianInterp.asc";
// CHECK THAT FILES EXIST!	
	ifstream processedFile(file);
	if (not processedFile.is_open())
	{
		cout << file << " does not exist\n"; 
		return;
	} 
	
	cout << "Compute baselines one, two and three\n";
	string line;
	getline(processedFile, line); // SKIP the files' headers
	
	ofstream outputfile;
	outputfile.open(outputfileName);
// FILE'S HEADER
	outputfile << "pp" << '\t' << "time" << '\t' <<  "wordNonWord" 
						<< '\t' << "trial" << '\t' << "item" 
						<< '\t' << "baseline1" << '\t' << "baseline2" 
						<< '\t' << "baseline3"
						<< '\t' << "fix\n";
// PROCESS EYETRACKING FILE
	// initialize variables
	string pp, item;
	size_t clockTime, trial, wordNonWord;
	double time; // time might be double in the interpolated lines
	float psize, fix;
	// all the values below are initiliazed to remove the warning: 
	// 	`b1_psize' may be used uninitialized in this function
	size_t oldTrial = 1;
	string oldPp = "";
	double b1_psize = 0;	
	double b2_psize = 0;	
	double b3_psize = 0;
	
// loop
	while (getline(processedFile, line))
	{
		istringstream linedata(line);
		linedata >> pp >> clockTime >> time >> wordNonWord >> 
			trial >> item >> psize >> fix; 
		// update baselines when eyetracking file goes from one trial to the next
		if (oldTrial != trial)
		{
			b1_psize = matchBaselines(pp, trial, (filePrefix + "0averageBeforeTarget200.asc"));
			b3_psize = matchBaselines(pp, trial, (filePrefix + "0average200_BeforeVisualOnset.asc"));
		}
		oldTrial = trial; // synchronizing to  eyetracking file
		
		if (oldPp != pp)
			b2_psize = matchBaselines(pp, 1, (filePrefix + "0averageFirst200.asc"));
		oldPp = pp; // updating eyetracking file
		
		outputfile << pp  << '\t' << time  << '\t' << wordNonWord  
			<< '\t' << trial << '\t' << item  << '\t' 
			<< (((psize - b1_psize) / b1_psize) * 100) << '\t' 
			<< (((psize - b2_psize) / b2_psize) * 100) << '\t' 
			<< (((psize - b3_psize) / b3_psize) * 100) << '\t' 
			<< fix	<< '\n';
	}
	
	outputfile.close();
	processedFile.close();
	return;
}
