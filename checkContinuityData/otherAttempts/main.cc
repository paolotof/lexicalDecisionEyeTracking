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
	string line2;
	while (true){
		if (processedFile.eof()) 
			break;
		getline(processedFile, line1);
		if (processedFile.eof()) 
			break;
		getline(processedFile, line2);
		// doing it only on line1 is worthless
		string item;
		size_t pp, clockTime, wordNonWord, trial, fix;
		double time, psize;
		istringstream linedata(line1);
		linedata >> pp >> clockTime >> time >> wordNonWord >> trial >> item 
			>> psize >> fix;
		string item2;
		size_t pp2, clockTime2, wordNonWord2, trial2, fix2;
		double time2, psize2;
		linedata.str(line2);
		linedata >> pp2 >> clockTime2 >> time2 >> wordNonWord2 >> trial2 >> item2 
			>> psize2 >> fix2;
		outFile << pp << trial << (time2 - time);
	}
	outFile.close();
	processedFile.close();
	return 1;
}
catch (...) { return 1; }
