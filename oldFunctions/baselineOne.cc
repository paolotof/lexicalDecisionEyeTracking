#include "main.ih"

void baselineOne()
{
	cout << "This is baseline one\n\n";
	cout << "Baseline one is computed on each trial for each participant\n";
	
	ifstream processedFile("tbt__0_ALL_medianInterp.asc");
	if (not processedFile.is_open())
	{
		cout << "tbt__0_ALL_medianInterp.asc does not exist\n"; 
		return;
	} else {
		ifstream baselineFile("tbt__0average200_BeforeVisualOnset.asc");
		if (not baselineFile.is_open())
		{
			cout << "tbt__0average200_BeforeVisualOnset.asc does not exist\n"; 
			return;
		} else {
			string line;
			// read the headers
			getline(processedFile, line);
			getline(baselineFile, line);
// read the first lines to see where the files are
			string pp, item;
			size_t clockTime, wordNonWord,	trial;
			float time,	psize,	fix;
			getline(processedFile, line);
			istringstream linedata(line);
			linedata >> pp >> clockTime >> time >> wordNonWord >> trial
				>> item >> psize >> fix; 
			size_t oldTrial = trial;
// baseline
			double b1_psize;	
			b1_psize = findMatchingBaseline(pp, trial, baselineFile);
			// FIRST LINE			
			ofstream outputfile;
			outputfile.open("tbt_eyeData_BaselineOne.asc");
			outputfile << "pp" << '\t' << "time" << '\t' <<  "wordNonWord" 
				<< '\t' << "trial" << '\t' << "item" << '\t' << "psize"  
				<< '\t' << "fix\n";
				
			outputfile << pp  << '\t' << time  << '\t' << wordNonWord  
				<< '\t' << trial << '\t' << item  << '\t' << 
				(((psize - b1_psize) / b1_psize) * 100)  
				<< '\t' << fix	<< '\n';
// FOLLOWING LINES			
			while (getline(processedFile, line))
			{
				istringstream linedata(line);
				linedata >> pp >> clockTime >> time >> wordNonWord >> trial
					>> item >> psize >> fix; 
				
				outputfile << pp  << '\t' << time  << '\t' << wordNonWord  
					<< '\t' << trial << '\t' << item  << '\t' << 
					(((psize - b1_psize) / b1_psize) * 100)  
					<< '\t' << fix	<< '\n';
				
				// new trial update baseline
				if (oldTrial != trial)
					b1_psize = findMatchingBaseline(pp, trial, baselineFile);
				oldTrial = trial; // updating eyetracking file
			}
			
			outputfile.close();
			processedFile.close();
			baselineFile.close();
			return;
		}
	}
}
