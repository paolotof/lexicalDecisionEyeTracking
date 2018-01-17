#include "main.ih"

int main(int argc, char **argv)
try
{
	string dir2save = "";
	string filePrefix = "RT_wrongR2_";
	string outputfile = dir2save + filePrefix;
	string TrialInfoFile = "trials2beIncluded.txt";
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
// 	TrialInfoFile = checkBlinksAndGetInterpBounds(TrialInfoFile, outputfile, lock2);
	files filenames;
	filenames = checkBlinksAndGetInterpBounds(TrialInfoFile, outputfile, lock2);
	
// 	size_t timeBefore = 1000;
	size_t timeBefore = 500; // check if this is used in the function
	// trialInfo::setTargetLimits
// 	timeBefore = 0; // now we are starting at the onset of the word
	size_t limit4extraction = 4000;
	outputfile += std::to_string(timeBefore);
	
// 	TrialInfoFile.replace(TrialInfoFile.find(".txt"), 4, "_noBlinks.txt");
// 	see line 20
	TrialInfoFile = filenames.TrialInfoFile;
	string interpolationFile = filenames.nameOutputfile; 
	
	interpolationFile = eliminateRedundantTrials(TrialInfoFile, interpolationFile);
	if (interpolationFile.find("none") != string::npos)
		return 0;
	
// // 	processData(TrialInfoFile, outputfile, timeBefore, 
// // 							lock2, limit4extraction, filePrefix);
// 	processData(filenames, outputfile, timeBefore, 
// 							lock2, limit4extraction, filePrefix);
	
// 	//extractSaccades(TrialInfoFile, outputfile, lock2);
// 	string tmp = outputfile;
// 	tmp.append("_ALL_medianInterp.asc");
// 	binData(tmp, 5); //lines2bin = 5
// 	lock2 = "onsetSoundStim";
// 	cout << "Baseline Before " << lock2 << " 1st trial ONLY\n\n";
// 	size_t baselineInt = 500;
// // 	size_t baselineInt = timeBefore; // does not work
// 	averageFirst200(outputfile, baselineInt, lock2);
// 	cout << "Baseline Before " << lock2 << " Every Trial\n\n";
// 	average200BeforeVisualOnset(outputfile, baselineInt, lock2);
// 	cout << "Baseline Before Every Word onset\n\n";	
// 	string eye2read = outputfile +
// 		"_ALL_medianInterp.asc";
// 	baselineBeforeTarget(outputfile, eye2read, baselineInt);
// 	string outputfileName = filePrefix + "eyeData_Baseline123.asc";
// 	ERDD_withBase123(filePrefix, outputfileName, timeBefore);
// 	binData(outputfileName, 5); //lines2bin = 5
}
catch (...)
{
    return 1;
}
