#include "main.ih"
/*

read interpolation file to see if the current subject and trial should be
interpolated.
The interpolation should be true only if a match between the current participant 
and the current trial is found.

 */ 

Interpdata currentInterpInfo(TrialInfo trialSet, files filenames){
	
	ifstream interpInfo(filenames.nameOutputfile);
	Interpdata interpolation;
	interpolation.setDoInterpolation(false);
	if (not interpInfo.is_open()) {
		cout << "NO interpolation file " << filenames.nameOutputfile << "\n";
		return interpolation; 
	} else {
		while (trialSet.g_subject() != interpolation.sub()
			&& trialSet.g_currentTr() != interpolation.nTrial()
			&& not interpInfo.eof()){
			string line1, line2;
			getline(interpInfo, line1); // reads 2 lines at the time because paired
			getline(interpInfo, line2);
			interpolation.setSub("none");
			if (interpInfo.good()){
				interpolation.setDoInterpolation(true);
				interpolation = interpolation.extractInterpData(line1, line2); } } // end while
	} // end else
	interpInfo.close(); 
	return(interpolation); }
