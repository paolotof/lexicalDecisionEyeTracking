#include "main.ih"
/* 
 * remove trials that should not be interpolated from the interpolation file
*/

string eliminateRedundantTrials(files filenames) {
	cout << "deleting trials which do not match between interpInfo and trialInfoFile\n";
	ifstream trialInfoFile(filenames.TrialInfoFile);
	if (not trialInfoFile.is_open())	{
		cout << "Unable To Open "<< filenames.TrialInfoFile << '\n';
		return "none";	}
	ifstream interpInfo(filenames.nameInterpolationFile);
	if (not interpInfo.is_open())	{
		cout << "NO interpolation file "<< filenames.nameInterpolationFile << '\n';
		return "none"; 	}
	string newInterpolationFile = "clean_"; 
	newInterpolationFile += filenames.nameInterpolationFile;
	ofstream outputfile(newInterpolationFile);
	if (not outputfile.is_open())	{
		cout << "NO OUTPUT FILE\n";
		return "none";	}
	string subTrial = "none";
	int numTrial = 0;
	
	while (true)	{
		if (interpInfo.eof())
			break;
		string line1, line2;
		getline(interpInfo, line1);
		getline(interpInfo, line2);
		istringstream secondLine(line2);
		string interpSub;
		int interpTrial;
		secondLine >> interpSub >> interpTrial;
		interpSub.erase(interpSub.end()-4,interpSub.end()); // remove '.asc' from name
		string line;
// 		while (true)
		if (( subTrial == interpSub) && (numTrial == interpTrial) ) {
			outputfile << line1 << '\n';
			outputfile << line2 << '\n';
		} else {
			while (getline(trialInfoFile, line)) {
	// 			getline(trialInfoFile, line);
				istringstream trialLine(line);
	// 			string subTrial;
	// 			int numTrial;
				trialLine >> subTrial >> numTrial;
				if ( (subTrial == interpSub) && (numTrial == interpTrial) ) {
					outputfile << line1 << '\n';
					outputfile << line2 << '\n';
					// multiple lines might have interpolation information for one trial
					break; }
				if (subTrial > interpSub) 
					break;
				if ((subTrial == interpSub) && (numTrial > interpTrial))
					break;
	// 			if (trialInfoFile.eof())
	// 				break;
			}
		}		
	}	
	
	interpInfo.close();
	trialInfoFile.close();
	outputfile.close();
	return newInterpolationFile;
	
// 	while (true)
// 	{
// 		if (trialInfoFile.eof())
// 			break;
// 		string line;
// 		getline(trialInfoFile, line);
// 		istringstream trialLine(line);
// 		string subTrial;
// 		int numTrial;
// 		trialLine >> subTrial >> numTrial;
// 		
// 		while (true)
// 		{
// 			if (subTrial < interpSub)
// 				break;
// 			if ((subTrial == interpSub) &&
// 				(numTrial < interpTrial) )
// 				break;
// 			if (interpInfo.eof())
// 				break;
// 			
// 			string line1, line2;
// 			getline(interpInfo, line1);
// 			getline(interpInfo, line2);
// 			istringstream secondLine(line2);
// 			string interpSub;
// 			int interpTrial;
// 			secondLine >> interpSub >> interpTrial;
// 			interpSub.erase(interpSub.end()-4,interpSub.end()); // remove '.asc' from name
// 			
// 			if ( (subTrial == interpSub) &&
// 				(numTrial == interpTrial) )
// 			{
// 				outputfile << line1 << '\n';
// 				outputfile << line2 << '\n';
// 				// multiple lines might have interpolation information for one trial
// 			}
// 		}
// 	}	
	
}
