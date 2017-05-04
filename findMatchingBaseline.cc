#include "main.ih"

double findMatchingBaseline(string& pp, size_t trial, ifstream& baselineFile)
{
	string line;
	string b_pp;
	size_t b_trial;
	double b_psize;
	
	while (true)
	{
		getline(baselineFile, line);
		istringstream lineBase(line);
		lineBase >> b_pp >> b_trial >> b_psize;
		b_pp = b_pp.erase(b_pp.find(".asc"), 4);
		if ((b_trial == trial) and (b_pp == pp))
			break;
	}
	return(b_psize);
}
// 		cout << "B tr " << trial << " b_trial " << b_trial << '\n';
