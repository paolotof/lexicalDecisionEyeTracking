#include "main.ih"

// void checkInterpolations(string fillersNoFillers){
void checkInterpolations(files filenames){
	cout << "checking interpolations\n";
// 	string fileName = "clean_RT" + fillersNoFillers 
// 	+ "_blinksPerSubjectSmaller300.txt";
// 	ifstream interpinfo(fileName);
	ifstream interpinfo(filenames.nameInterpolationFile);
	if (! interpinfo.is_open()){
		cout << "Unable to open "<< filenames.nameInterpolationFile << '\n';
		return; }
	string fileName = "XXX_wrongR2_500_ALL_medianInterp.asc";
	ifstream eyedata(fileName);
	if (! eyedata.is_open()){
		cout << "Unable to open "<< fileName << '\n';
		return; }
	fileName = "reportInterpolations.txt";
	ofstream reportInt(fileName);
// skip eye's data file header and initialize first line of data
	string dataline;
	getline(eyedata, dataline);
	getline(eyedata, dataline);
	istringstream linedata(dataline); 
	string currentPp, condition, item, exp;
	size_t clockTime, currentTr;
	int time; // time can also be negative, but it will always be an integer
	double psize, target, competitor, d1, d2, fix;
	linedata >> currentPp >> clockTime >> time >> condition 
		>> currentTr >> item >> exp >> psize >> target >> competitor 
		>> d1 >> d2 >> fix;
	
// loop through the files to match interpolation with eyedata	
	string interpline1;
	Interpdata interpolation;
	while (getline(interpinfo, interpline1)){
		string interpline2;
		if (interpinfo.eof()){
			cout << "interpinfo file ended\n";
			return;
		}
		getline(interpinfo, interpline2);
		if (interpinfo.eof()){
			cout << "interpinfo file ended\n";
			return;
		}
		interpolation = interpolation.extractInterpData(interpline1, interpline2);
// 		while (true) {
// 			getline(eyedata, dataline);
		while (getline(eyedata, dataline)){
// 			if (eyedata.eof()){
// 				cout << "eyedata file ended\n";
// 				return;
// 			}
			istringstream linedata(dataline);
			linedata >> currentPp >> clockTime >> time >> condition 
				>> currentTr >> item >> exp >> psize >> target >> competitor 
				>> d1 >> d2 >> fix;
			if ((currentPp == interpolation.sub()) 
				&& (currentTr == interpolation.nTrial())
// 				&& (clockTime == interpolation.iEnd()) // some times it does not match
				// because it skips the last line
// 				&& (clockTime == interpolation.iBegin()) // with this one it works even worst
				&& (clockTime >= interpolation.iBegin()) // with this one it works even worst
				&& (clockTime <= interpolation.iEnd()) // with this one it works even worst
			) {
				reportInt << dataline << '\n';
				reportInt << interpline2 << '\n';
				break; // exit from the loop and update, I think it should exit at the first match
			} // if ((currentPp == interpolation.sub())
		} // while (getline(eyedata, dataline)){
// 		cout << "eyedata file ended\n";
	}
	interpinfo.close();
	eyedata.close();
	reportInt.close();
	cout << "interpolations check out!!\n";
	return;
}
