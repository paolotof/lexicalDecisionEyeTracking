#include "main.ih"

void baselineTwo()
{
	cout << "This is baseline TWO\n\n";
	cout << "Baseline TWO is at the beginning of the experiment\n";
	
	ifstream processedFile("tbt__0_ALL_medianInterp.asc");
	if (not processedFile.is_open())
	{
		cout << "tbt__0_ALL_medianInterp.asc does not exist\n"; 
		return;
	} else {
		ifstream baselineFile("tbt__0averageFirst200.asc");
		if (not baselineFile.is_open())
		{
			cout << "tbt__0averageFirst200.asc does not exist\n"; 
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
			linedata >> pp >> clockTime >> time >> wordNonWord 
				>> trial >> item >> psize >> fix; 
			string oldPp = pp;
			// baseline
			double b2_psize;	
			b2_psize = findMatchingPp(pp, baselineFile);
// FIRST LINE			
			ofstream outputfile;
			outputfile.open("tbt_eyeData_BaselineTwo.asc");
			outputfile << "pp" << '\t' << "time" << '\t' <<  "wordNonWord" 
				<< '\t' << "trial" << '\t' << "item" << '\t' << "psize"  
				<< '\t' << "fix\n";
			
			outputfile << pp  << '\t' << time  << '\t' << wordNonWord  
				<< '\t' << trial << '\t' << item  << '\t' << 
				(((psize - b2_psize) / b2_psize) * 100)  
				<< '\t' << fix	<< '\n';
// FOLLOWING LINES			
			while (getline(processedFile, line))
			{
				istringstream linedata(line);
				linedata >> pp >> clockTime >> time >> wordNonWord 
					>> trial >> item >> psize >> fix; 
				
				outputfile << pp  << '\t' << time  << '\t' << wordNonWord  
					<< '\t' << trial << '\t' << item  << '\t' << 
					(((psize - b2_psize) / b2_psize) * 100)  
					<< '\t' << fix	<< '\n';
				
				// new pp update baseline
				if (oldPp != pp)
					b2_psize = findMatchingPp(pp, baselineFile);
				oldPp = pp; // updating eyetracking file
			}
			
			outputfile.close();
			processedFile.close();
			baselineFile.close();
			return;
		}
	}	
}
