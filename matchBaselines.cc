#include "main.ih"
/* 
	This file `improves' on findingMatchingPp and findingMatchingBaselines because
	it read in the file every time and read through all of it to find the matching 
	baseline. This approach is not efficient, but I hope it is less subjective to 
	failure in matching the two files since it loops through all the data, find the
	match and move on
 */ 
double matchBaselines(string& pp, size_t trial, string baseline2read)
{
	
	ifstream baselineFile(baseline2read);
	if (not baselineFile.is_open())
	{
		cout << baseline2read << " does not exist\n"; 
		return 0;
	}
	string line;
	getline(baselineFile, line); // SKIP the files' headers
	
	string b_pp;
	size_t b_trial;
	double b_psize;
	
	while (getline(baselineFile, line))
	{
		istringstream lineBase(line);
		lineBase >> b_pp >> b_trial >> b_psize;
		if (b_pp.find(".asc") != string::npos)
			b_pp = b_pp.erase(b_pp.find(".asc"), 4);
		if ((b_trial == trial) and (b_pp.compare(pp) == 0))
			break;
	}
	baselineFile.close();
	return(b_psize);
}
// 		cout << "B tr " << trial << " b_trial " << b_trial << '\n';
