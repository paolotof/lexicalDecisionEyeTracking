#include "main.ih"

void baselineOneAndTwo(string& outputfileName)
{
	
	ifstream processedFile("tbt__0_ALL_medianInterp.asc");
	if (not processedFile.is_open())
	{
		cout << "tbt__0_ALL_medianInterp.asc does not exist\n"; 
		return;
	} 
	
	ifstream baseline1File("tbt__0average200_BeforeVisualOnset.asc");
	if (not baseline1File.is_open())
	{
		cout << "tbt__0average200_BeforeVisualOnset.asc does not exist\n"; 
		return;
	}	

	ifstream baseline2File("tbt__0averageFirst200.asc");
	if (not baseline2File.is_open())
	{
		cout << "tbt__0averageFirst200.asc does not exist\n"; 
		return;
	}
	
	cout << "Compute baselines one and two\n";
	string line;
// read and SKIP the files' headers
	getline(processedFile, line);
	getline(baseline1File, line);
	getline(baseline2File, line);
// read the first lines to synchronize the files
	string pp, item;
	size_t clockTime, wordNonWord,	trial;
	float time,	psize,	fix;
	getline(processedFile, line);
	istringstream linedata(line);
	linedata >> pp >> clockTime >> time >> wordNonWord 
					 >> trial >> item >> psize >> fix; 
	size_t oldTrial = trial; // this is to update baseline1
	string oldPp = pp; // this is to update baseline2
// baselines	
	double b1_psize;	
	b1_psize = findMatchingBaseline(pp, trial, baseline1File);
	double b2_psize;	
	b2_psize = findMatchingPp(pp, baseline2File);
	
// FIRST LINE			
	ofstream outputfile;
	outputfile.open(outputfileName);
	outputfile << "pp" << '\t' << "time" << '\t' <<  "wordNonWord" 
						<< '\t' << "trial" << '\t' << "item" 
						<< '\t' << "baseline1" << '\t' << "baseline2" 
						<< '\t' << "fix\n";
	
	outputfile << pp  << '\t' << time  << '\t' << wordNonWord  
						<< '\t' << trial << '\t' << item  << '\t' 
						<< (((psize - b1_psize) / b1_psize) * 100) << '\t'  
						<< (((psize - b2_psize) / b2_psize) * 100) << '\t' 
						<< fix	<< '\n';
// FOLLOWING LINES			
	while (getline(processedFile, line))
	{
		istringstream linedata(line);
		linedata >> pp >> clockTime >> time >> wordNonWord >> trial
		>> item >> psize >> fix; 
		
		outputfile << pp  << '\t' << time  << '\t' << wordNonWord  
			<< '\t' << trial << '\t' << item  << '\t' 
			<< (((psize - b1_psize) / b1_psize) * 100) << '\t' 
			<< (((psize - b2_psize) / b2_psize) * 100) << '\t' 
			<< fix	<< '\n';
			
		// update baselines 
		if (oldTrial != trial)
			b1_psize = findMatchingBaseline(pp, trial, baseline1File);
		oldTrial = trial; // updating eyetracking file
		
		if (oldPp != pp)
			b2_psize = findMatchingPp(pp, baseline2File);
		oldPp = pp; // updating eyetracking file
		
	}
	
	outputfile.close();
	processedFile.close();
	baseline1File.close();
	baseline2File.close();
	return;
}
