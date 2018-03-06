#include "main.ih"


// does not seem to work for multiple blinks in one trials


/* This file does two things:
 * 1 - 	a) creates a file containing blinks longer than 300 ms. These trials are excluded from
 * further analysis
 * 	b) updates the file with the 'trials2beIncluded' excluding the trials with blinks longer 
 * than 300 ms. Trials not included in that file are excluded from further analysis. The information
 * for inclusion or exclusion is decided when the 'TRIAL ENDS' message is encounterd.
 * 
 * 2 - Export informations about interpolation of blinks using median of N data lines
 * remove trials with blinks longer than 300 ms
 * leave alone trials with blinks shorter than 20 ms (the will be interpolated linearly)
 * compute median of 25 line before and after blink for boundaries of linear interpolation for
 * blinks longer than 20 ms and shorter than 300
 * 
 * 
 * 
 */

// files checkBlinksAndGetInterpBounds(string TrialInfoFile, string nameInterpolationFile, 
// 																		 string& lock2){
files checkBlinksAndGetInterpBounds(files filenames, string& lock2){
	cout << "Blinks and interpolation files used\n" 
	<< filenames.TrialInfoFile << ' ' 
	<< filenames.nameInterpolationFile << '\n';
	ifstream fileWithTrialInfo(filenames.TrialInfoFile);
	if (not fileWithTrialInfo.is_open()) {
		cout << "Unable to open " << filenames.TrialInfoFile << '\n';
		files filenames;
		return (filenames);}
	filenames.TrialInfoFile.replace(filenames.TrialInfoFile.find(".txt"), 4, "_noBlinks.txt");
  ofstream updatedTrialInfoFile;
  updatedTrialInfoFile.open(filenames.TrialInfoFile);
	
// 	filenames.nameInterpolationFile.append("blinksPerSubjectSmaller300.txt"); 
//   ofstream shortBlinksReport;
// 	shortBlinksReport.open(nameInterpolationFile);  
// 	cout << nameInterpolationFile << '\n';
	string trialInfo;
  TrialInfo trialSet;
  while (getline(fileWithTrialInfo, trialInfo))  {
    trialSet.extractInfo(trialInfo);
    string subNum = trialSet.g_subject();  
    ifstream eyetrackingFile(subNum.append(".asc"));
    if (not eyetrackingFile.is_open()) {
      cout << "Unable to open eyetracking datafile " << subNum << ' ';
      break;
    } else {
      cout << trialSet.g_subject() << ' ';
      trialSet.setCurrentTr(0);
      bool includeTrial = true;
      vector<double> tVect, xVect, yVect, pVect;
// 			size_t nLines2interpolate = 50; // lines to include for       
			size_t nLines2interpolate = 25; // lines to include for interpolation
			// this gives a warning
			//double medianT, medianX, medianY, medianP;
			double medianT = -1;
			double medianX = -1; 
			double medianY = -1; 
			double medianP = -1;
			string line;
      Dataline eye;
      //size_t onsetWord = 0;
      double blinkDuration = 0; 
			// subject 8 gets out of the while loop before the file is finished!
      while (getline(eyetrackingFile, line)) {
// TRIAL STARTS
				if (line.find(lock2) != string::npos)	
					trialSet.updateCurrentTrial(line);

				if (trialSet.g_currentTr() == trialSet.g_trialIN()){
// TRIAL ENDS - update trial information/reset arrays
					if (line.find("TRIAL ENDS") != string::npos) {
						if (includeTrial)
							updatedTrialInfoFile << trialInfo << '\n';
						includeTrial = true; // reset include trials
						blinkDuration = 0;
						getline(fileWithTrialInfo, trialInfo); 
						trialSet.extractInfo(trialInfo);
						// if the current subject does not have trials that should be 
						// considered anymore stop with this file and get to the next one
						if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject()){
// 							cout << '\n' << subNum << " processing ends at trial " 
// 									 << trialSet.g_currentTr() << '\n';
							eyetrackingFile.close();
							break;
						} // if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject())
						
						vector<double>().swap(tVect);
						vector<double>().swap(xVect);
						vector<double>().swap(yVect);
						vector<double>().swap(pVect);} // if ((line.find("TRIAL ENDS") != string::npos)

// BLINK STARTS - get median of previously stored values
					if (line.find("SBLINK") != string::npos) {
						medianT = vectorMedian(tVect);
						medianX = vectorMedian(xVect);
						medianY = vectorMedian(yVect);
						medianP = vectorMedian(pVect);
						// reset vector 
						vector<double>().swap(tVect);
						vector<double>().swap(xVect);
						vector<double>().swap(yVect);
						vector<double>().swap(pVect);} // if (line.find("SBLINK") != string::npos)
// BLINK ENDS	
					if (line.find("EBLINK") != string::npos){
						istringstream linedata(line); // extract time information from the datafile
						string msgType, side;
						double blinkBegin, blinkEnd, durationBlink;
						linedata >> msgType >> side >> blinkBegin >> blinkEnd >> durationBlink;
						
						if ((blinkEnd >= trialSet.g_startExport()) 
							&& (blinkBegin <= trialSet.timeIsUp())){
							
							// blinkDuration = blinkEnd - blinkBegin;
							blinkDuration = durationBlink;
							// limit blink durations to the boundaries of the time interval of interest
							// 15-2-2018 removed the equal signs from the boundaries
							if (blinkEnd > trialSet.timeIsUp())
								blinkDuration = trialSet.timeIsUp() - blinkBegin;
							if (blinkBegin < trialSet.g_startExport())
								blinkDuration = blinkEnd - trialSet.g_startExport();
							// 2) If length of the blink from onset word is 
							// 	2.b > 300 ms - exclude 
							//    2.b < 300 ms - interpolate
							
							bool exportInterpolationData;
							if (blinkDuration > 300){
								includeTrial = false;
							} else {
								// if includeTrial is already false none of this should be done!
								// moreover, we do not what interpolation information for blinks shorter than 20ms
								if (includeTrial && (blinkDuration > 20)){
									// includeTrial is not set true again because if there is one blink that is 
									// longer than 300 ms the trial should not be included also if the blinks 
									// after are shorter than 300 ms
									exportInterpolationData = true;
									// get other 25 lines of data to get the median of, however mind whether 
									// 1 - a new blinks starts
									// 2 - a new trial starts
									for (size_t lines2skip = 0; lines2skip < nLines2interpolate; lines2skip++) {
										getline(eyetrackingFile, line);
										// Before nLines2interpolate are included in the array it can happen that:
// 1 trial Ends
// 2 a new blink starts
// 3 data are just added
// 1										
										if (line.find("TRIAL ENDS") != string::npos) {
											// we should still export interpolation information even if we do not have 25
											// lines
											if (tVect.size() > 1)
												exportInterpolation(filenames.nameInterpolationFile, blinkDuration, trialSet, 
																						vectorMedian(tVect), vectorMedian(xVect), vectorMedian(yVect), vectorMedian(pVect), 
																						medianT, medianX, medianY, medianP);//, 'S');	
											// interrupt all extrapolation for current blink
											vector<double>().swap(tVect);
											vector<double>().swap(xVect);
											vector<double>().swap(yVect);
											vector<double>().swap(pVect);
											// do all the operation for 'trials2beIncluded' file update, since other blinks 
											// in this current trial should be included
											updatedTrialInfoFile << trialInfo << '\n';
											getline(fileWithTrialInfo, trialInfo); 
											trialSet.extractInfo(trialInfo);
											exportInterpolationData = false; // interrupt interpolation for this specific blink
											
											/* this creates problem because _blinksPerSubjectSmaller300 has different trials
											*  then trials2beIncluded_x_noBlinks file. Therefore it's preferable to read the 
											*  whole eyetracking file
											*/
											if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject()){
												cout << '\n' << subNum << " TRIALS ENDS, non match " 
													<< trialSet.g_currentTr() << '\n';
												eyetrackingFile.close();
												break; } // END: if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject())
											break; } // END: if (line.find("TRIAL ENDS") != string::npos)
	// 2									
	// in case a blink happens before 25 lines
	// export what you have up to here if you have more than one line!
										if (line.find("SBLINK") != string::npos) {
											if (tVect.size() > 1)
												exportInterpolation(filenames.nameInterpolationFile, blinkDuration, trialSet, 
																						vectorMedian(tVect), vectorMedian(xVect), vectorMedian(yVect), vectorMedian(pVect), 
																						medianT, medianX, medianY, medianP); //, 'S');	
											
											// update the line for the new interpolation
											medianT = vectorMedian(tVect);
											medianX = vectorMedian(xVect);
											medianY = vectorMedian(yVect);
											medianP = vectorMedian(pVect);
											// reset vector 
											vector<double>().swap(tVect);
											vector<double>().swap(xVect);
											vector<double>().swap(yVect);
											vector<double>().swap(pVect);
											// fish a new line and break
											getline(eyetrackingFile, line);
											exportInterpolationData = false;
	// 										cout << "New blink starts before enough trials to interpolate\n";
											cout << '.';
											break; } //if (line.find("SBLINK") != string::npos)
										// update array with data for postblink interpolation
	// 3 DATA ARE JUST ADDED									
										eye.extractData(line);
										if (not eye.isMSG() && eye.isValid()){
											tVect.push_back(eye.g_time());
											xVect.push_back(eye.g_xpos());
											yVect.push_back(eye.g_ypos());
											pVect.push_back(eye.g_psize()); } // if (not eye.isMSG() && eye.isValid())
									} // END: for (size_t lines2skip = 0; lines2skip < nLines2interpolate; lines2skip++) 
// Export data only if 'TRIAL ENDS' or 'SBLINK' messages were not encountered
									if (exportInterpolationData) {
										exportInterpolation(filenames.nameInterpolationFile, blinkDuration, trialSet, 
												vectorMedian(tVect), vectorMedian(xVect), vectorMedian(yVect), vectorMedian(pVect), 
												medianT, medianX, medianY, medianP); //, 'X');	
										vector<double>().swap(tVect); // 
										vector<double>().swap(xVect);
										vector<double>().swap(yVect);
										vector<double>().swap(pVect);
									} // END: if (exportInterpolationData) 
								} // END: if includeTrial
							} // END: if (blinkDuration > 300){
						} // END: if ((blinkEnd >= trialSet.g_startExport()) && (blinkBegin <= trialSet.timeIsUp()))
					} // end if ((line.find("EBLINK") != string::npos))
				} // END: TRIAL ENDS
// EXTRACT DATA: needs to be done anyways because it can happen that
// the interpolation needs to start before the "START" message 
				eye.extractData(line);
				if ((not eye.isMSG()) && (eye.isValid())){
					if (tVect.size() <= nLines2interpolate){
						tVect.push_back(eye.g_time());
						xVect.push_back(eye.g_xpos());
						yVect.push_back(eye.g_ypos());
						pVect.push_back(eye.g_psize());
					} else {
						tVect.push_back(eye.g_time());
						tVect.erase(tVect.begin());
						xVect.push_back(eye.g_xpos());
						xVect.erase(xVect.begin());
						yVect.push_back(eye.g_ypos());
						yVect.erase(yVect.begin());
						pVect.push_back(eye.g_psize());
						pVect.erase(pVect.begin()); }
				} // end of "if (not eye.isMSG() && eye.isValid()) 
      } // end of "while(getline(eyetrackingFile, line))"
    } // end "if(! eyetrackingFile.is_open()") 
    
    eyetrackingFile.close();
		if (fileWithTrialInfo.eof())
      break;
  } // end "while(getline(fileWithTrialInfo, trialInfo))"
  
  fileWithTrialInfo.close();
  updatedTrialInfoFile.close();
//   shortBlinksReport.close();
	cout << '\n';
// 	return TrialInfoFile; // nameInterpolationFile;
// 	files filenames;
// 	filenames.TrialInfoFile = TrialInfoFile;
// 	filenames.nameInterpolationFile = nameInterpolationFile;
	return filenames;
}

// 5945090
// 5946522
