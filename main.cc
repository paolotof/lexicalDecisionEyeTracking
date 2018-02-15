#include "main.ih"

int main(int argc, char **argv)
try
{
	string dir2save = "";
	string filePrefix = "XXX_wrongR2_";
	string outputfile = dir2save + filePrefix;
	string TrialInfoFile = "trials2beIncluded.txt";
	// these strings could also be added to the structure with the file names, it's cleaner
// 	string TrialInfoFile = "trials2beIncluded_withErrors.txt";
	if (argc == 2)
		TrialInfoFile = argv[1];
	if (argc > 2){
		cout << "Too many input arguments to call!!";
		return 0;
	}
	cout << "USING: " << TrialInfoFile <<'\n';
// 	return 0;
	string lock2 = "STARTS";
	
// 	to skip this step you need to define filanames see line: 46
	files filenames; 
	filenames = checkBlinksAndGetInterpBounds(TrialInfoFile, outputfile, lock2);
	
	size_t timeBefore = 500; // check if this is used in the function
	size_t limit4extraction = 4000;
	outputfile += std::to_string(timeBefore);
	
// 	TrialInfoFile = filenames.TrialInfoFile;
// 	string filenames.nameInterpolationFile = filenames.nameInterpolationFile; 
// 	
// 	THIS seems to remove too many trials, either check what's wrong or do not use
// 	filenames.nameInterpolationFile = eliminateRedundantTrials(filenames);
// 	if (filenames.nameInterpolationFile.find("none") != string::npos)
// 		return 0;
	
// 	return 0;
// 	cout << TrialInfoFile << '\n';
// 	cout << filenames.nameInterpolationFile << '\n';
// 	
// 	files filenames;
// 	filenames.TrialInfoFile = "trials2beIncluded_withErrors_noBlinks.txt";
// 	filenames.nameInterpolationFile = "clean_RT_wrongR2_blinksPerSubjectSmaller300.txt";

// 	return 0;
	
	newProcessData(filenames, outputfile, timeBefore, 
							lock2, limit4extraction, filePrefix);
	
	//extractSaccades(TrialInfoFile, outputfile, lock2);
// 	string tmp = outputfile;
// 	tmp.append("_ALL_medianInterp.asc");
// 	binData(tmp, 5); //lines2bin = 5
// 	cout << "\n\n ---- BASELINES ---- \n\n";
// 	lock2 = "onsetSoundStim";
// 	cout << "Baseline Before " << lock2 << " 1st trial ONLY\n\n";
// 	size_t baselineInt = 200;
// 	averageFirst200(outputfile, baselineInt, lock2);
// 	cout << "Baseline Before " << lock2 << " Every Trial\n\n";
// 	average200BeforeVisualOnset(outputfile, baselineInt, lock2);
// 	cout << "Baseline Before Every Word onset\n\n";	
// 	string eye2read = outputfile +
// 		"_ALL_medianInterp.asc";
// 	baselineBeforeTarget(outputfile, eye2read, baselineInt);
	
	return 0; // skip this, takes too long
// 	string outputfileName = filePrefix + "eyeData_Baseline123.asc";
// 	ERDD_withBase123(filePrefix, outputfileName, timeBefore, baselineInt);
// 	binData(outputfileName, 5); //lines2bin = 5
}
catch (...)
{
    return 1;
}
