#include "main.ih"

int main(int argc, char **argv)
try {
	string processedFileName = "XXX_wrongR2_500_ALL_medianInterp.asc";
	if (argc > 1)
		processedFileName = argv[1];
	ifstream processedFile(processedFileName);
	if (not processedFile.is_open()) {
		cout << "NO interpolation file " << processedFileName << "\n"; 
		processedFile.close(); 
		return 0; } 
	ofstream outFile(std::string("checked_").append(processedFileName));
	string line1;
	// read file's header
	getline(processedFile, line1);
	// read next line
	getline(processedFile, line1);
	string item;
	size_t pp, clockTime, wordNonWord, trial, fix, previousPP, previousTrial;
	double time, psize, previousTime;
	istringstream linedata(line1);
	linedata >> pp >> clockTime >> time >> wordNonWord >> trial >> item 
		>> psize >> fix;
	previousPP = pp;
	previousTrial = trial;
	previousTime = time;
	outFile << "pp trial diff\n";
	while (getline(processedFile, line1)){
		linedata.str("");
		linedata.clear();
		linedata.str(line1);
		linedata >> pp >> clockTime >> time >> wordNonWord >> trial >> item 
			>> psize >> fix;
// 		cout << pp;
		if (previousPP == pp && previousTrial == trial)
			outFile << pp << " " << trial << " "  << (time - previousTime) << '\n';
		
		previousPP = pp;
		previousTrial = trial;
		previousTime = time;
	}
	outFile.close();
	processedFile.close();
	return 1;
}
catch (...) { return 1; }
