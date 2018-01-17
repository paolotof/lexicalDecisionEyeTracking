#include "main.ih"

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

files checkBlinksAndGetInterpBounds(string TrialInfoFile, string nameOutputfile, 
																		 string& lock2){
	cout << "Blinks and interpolation\n";
	ifstream fileWithTrialInfo(TrialInfoFile);
	if (not fileWithTrialInfo.is_open()) {
		cout << "Unable to open " << TrialInfoFile << '\n';
		files filenames;
		return (filenames);}
	TrialInfoFile.replace(TrialInfoFile.find(".txt"), 4, "_noBlinks.txt");
  ofstream updatedTrialInfoFile;
  updatedTrialInfoFile.open(TrialInfoFile);
	
  ofstream longBlinksReport;
  longBlinksReport.open(nameOutputfile.append("blinksPerSubject.txt")); 
  ofstream shortBlinksReport;
	nameOutputfile.replace(nameOutputfile.find(".txt"), 4, "Smaller300.txt");
	shortBlinksReport.open(nameOutputfile);  
// 	cout << nameOutputfile << '\n';
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
      vector<double> tVect, xVect, vector<double> yVect, vector<double> pVect;
// 			size_t nLines2interpolate = 50; // lines to include for       
			size_t nLines2interpolate = 25; // lines to include for interpolation
      double medianT, double medianX, double medianY, double medianP;
      string line;
      Dataline eye;
      //size_t onsetWord = 0;
      double blinkDuration = 0;
      while (getline(eyetrackingFile, line)) {
			// BEGINNING OF WORD SOUND
				if (line.find(lock2) != string::npos)	
					trialSet.updateCurrentTrial(line);

// TRIAL ENDS - update trial information/reset arrays
				if ((line.find("TRIAL ENDS") != string::npos) && 
					(trialSet.g_currentTr() == trialSet.g_trialIN())){
					
					if (includeTrial)
						updatedTrialInfoFile << trialInfo << '\n';
					includeTrial = true; // reset include trials
					blinkDuration = 0;
					getline(fileWithTrialInfo, trialInfo); 
					trialSet.extractInfo(trialInfo);
					// if the current subject does not have trials that should be 
					// considered anymore stop with this file and get to the next one
					if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject()){
						cout << '\n' << subNum << " processing ends at trial " << trialSet.g_currentTr() << '\n';
						eyetrackingFile.close();
						break;
					} // if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject())
					
					vector<double>().swap(tVect);
					vector<double>().swap(xVect);
					vector<double>().swap(yVect);
					vector<double>().swap(pVect);} // if ((line.find("TRIAL ENDS") != string::npos) && (trialSet.g_currentTr() == trialSet.g_trialIN()))

// BLINK STARTS - get median of previously stored values
				if ((line.find("SBLINK") != string::npos) 
					&& (trialSet.g_currentTr() == trialSet.g_trialIN())) {
					medianT = vectorMedian(tVect);
					medianX = vectorMedian(xVect);
					medianY = vectorMedian(yVect);
					medianP = vectorMedian(pVect);
					// reset vector 
					vector<double>().swap(tVect);
					vector<double>().swap(xVect);
					vector<double>().swap(yVect);
					vector<double>().swap(pVect);
				}
// BLINK ENDS	
				if ((line.find("EBLINK") != string::npos) 
					&& (trialSet.g_currentTr() == trialSet.g_trialIN())){
					istringstream linedata(line); // extract time information from the datafile
					string msgType, side;
					double blinkBegin, blinkEnd;
					linedata >> msgType >> side >> blinkBegin >> blinkEnd;
					
					if ((blinkEnd >= trialSet.g_startExport()) 
						&& (blinkBegin <= trialSet.timeIsUp())){
						blinkDuration = blinkEnd - blinkBegin;
						if (blinkEnd >= trialSet.timeIsUp())
							blinkDuration = trialSet.timeIsUp() - blinkBegin;
						if (blinkBegin <= trialSet.g_startExport())
							blinkDuration = blinkEnd - trialSet.g_startExport();
						// 2) If length of the blink from onset word is 
						// 	2.b > 300 ms - exclude 
						//    2.b < 300 ms - interpolate
						
						bool exportData;
						if (blinkDuration < 300){
							includeTrial = true;
							exportData = true;
							// get other 25 lines of data to get the median of
							for (size_t lines2skip = 0; lines2skip < nLines2interpolate; lines2skip++) {
								getline(eyetrackingFile, line);
								if (line.find("TRIAL ENDS") != string::npos) {
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
									exportData = false; // interrupt interpolation for this specific blink
									
									/* this creates problem because _blinksPerSubjectSmaller300 has different trials
									*  then trials2beIncluded_x_noBlinks file. Therefore it's preferable to read the 
									*  whole eyetracking file
									*/
									if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject()){
// 												cout << '\n' << subNum << " processing ends at trial " << trialSet.g_currentTr() << '\n';
										eyetrackingFile.close();
										break; } // if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject())
									break; } // breaks the for loop if (line.find("TRIAL ENDS") != string::npos)
								
// in case a blink happens before 25 lines
// export what you have up to here if you have more than one line!
								if (line.find("SBLINK") != string::npos) {
									if (tVect.size() > 1){
										shortBlinksReport << subNum << ' ' << trialSet.g_currentTr() << ' '; 
										size_t old_precision = shortBlinksReport.precision();
										shortBlinksReport.precision(10);
										shortBlinksReport << medianT ;
										shortBlinksReport.precision(old_precision);
										shortBlinksReport << ' ' << medianX << ' ' << medianY << ' ' << medianP << '\n';
										// end interpolation
										shortBlinksReport << subNum << ' ' << trialSet.g_currentTr() << ' ';
										shortBlinksReport.precision(10);
										shortBlinksReport << vectorMedian(tVect); 
										shortBlinksReport.precision(old_precision);
										shortBlinksReport << ' ' << vectorMedian(xVect) 
										<< ' ' << vectorMedian(yVect)  << ' ' << vectorMedian(pVect)  
										<< ' ' << blinkDuration / 4 << '\n'; // this last line represents the lines to interpolate
									}
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
									exportData = false;
									break;
								} //if (line.find("SBLINK") != string::npos)
								// update array with data for postblink interpolation
								eye.extractData(line);
								if (not eye.isMSG() && eye.isValid())								{
									tVect.push_back(eye.g_time());
									xVect.push_back(eye.g_xpos());
									yVect.push_back(eye.g_ypos());
									pVect.push_back(eye.g_psize()); } // if (not eye.isMSG() && eye.isValid())
							} // for (size_t lines2skip = 0; lines2skip < nLines2interpolate; lines2skip++) 
							// for loop to collect data after end of the blink
							
// Export data only if 'TRIAL ENDS' message wasn't encountered
							if (exportData) {
								// begin interpolation
								shortBlinksReport << subNum << ' ' << trialSet.g_currentTr() << ' '; 
								size_t old_precision = shortBlinksReport.precision();
								shortBlinksReport.precision(10);
								shortBlinksReport << medianT ;
								shortBlinksReport.precision(old_precision);
								shortBlinksReport << ' ' << medianX << ' ' << medianY << ' ' << medianP << '\n';
								// end interpolation
								shortBlinksReport << subNum << ' ' << trialSet.g_currentTr() << ' ';
								shortBlinksReport.precision(10);
								shortBlinksReport << vectorMedian(tVect); 
								shortBlinksReport.precision(old_precision);
								shortBlinksReport << ' ' << vectorMedian(xVect) 
									<< ' ' << vectorMedian(yVect)  << ' ' << vectorMedian(pVect)  
									<< ' ' << blinkDuration / 4 << '\n'; // this last line represents the lines to interpolate
								// it is divided by 4 because the blink duration is in ms but it should be translated into 
								// lines to interpolate
								vector<double>().swap(tVect);
								vector<double>().swap(xVect);
								vector<double>().swap(yVect);
								vector<double>().swap(pVect);
							} // end if (exportData) 
						// end if-else (blinkDuration > 300)	    
						} 
						// remove the else because we never really do much with it and then the code is shorter
// 						else {
// // 								cout << '-';
// 							longBlinksReport << subNum << ' ' << trialSet.g_currentTr() << ' ' << blinkDuration << '\n'; 
// 							includeTrial = false; 
// 							// Skip remaining lines of this current trial. Go to the next trial searching for 'TRIAL ENDS' 
// 							while (getline(eyetrackingFile, line))
// 							{
// 								//cout << line << '\n';
// 								if (line.find("TRIAL ENDS") != string::npos)
// 								{
// 									blinkDuration = 0;
// 									getline(fileWithTrialInfo, trialInfo); 
// 									trialSet.extractInfo(trialInfo);
// 									vector<double>().swap(tVect);
// 									vector<double>().swap(xVect);
// 									vector<double>().swap(yVect);
// 									vector<double>().swap(pVect);
// 									break;
// 								} // if (line.find("TRIAL ENDS") != string::npos)
// 							} // end 'while (getline(eyetrackingFile, line))'
// 						} // end if-else - if (blinkDuration < 300)
					}
					else
					{
						includeTrial = true; 
					} // if ((blinkEnd >= trialSet.g_targetOnset()) && (blinkBegin <=   trialSet.timeIsUp()))
				} // end if ((line.find("EBLINK") != string::npos) && (trialSet.g_currentTr() == trialSet.g_trialIN()))

			// EXTRACT DATA	
				eye.extractData(line);
				if ((not eye.isMSG()) && (eye.isValid()))
				{
					if (tVect.size() <= nLines2interpolate)
					{
						tVect.push_back(eye.g_time());
						xVect.push_back(eye.g_xpos());
						yVect.push_back(eye.g_ypos());
						pVect.push_back(eye.g_psize());
					}
					else
					{
						tVect.push_back(eye.g_time());
						tVect.erase(tVect.begin());
						xVect.push_back(eye.g_xpos());
						xVect.erase(xVect.begin());
						yVect.push_back(eye.g_ypos());
						yVect.erase(yVect.begin());
						pVect.push_back(eye.g_psize());
						pVect.erase(pVect.begin());
					}
				} // end of "if (not eye.isMSG() && eye.isValid()) 
      } // end of "while(getline(eyetrackingFile, line))"
    } // end "if(! eyetrackingFile.is_open()") 
    
    eyetrackingFile.close();
		if (fileWithTrialInfo.eof())
      break;
  } // end "while(getline(fileWithTrialInfo, trialInfo))"
  
  fileWithTrialInfo.close();
  updatedTrialInfoFile.close();
  longBlinksReport.close();
  shortBlinksReport.close();
	cout << '\n';
// 	return TrialInfoFile; // nameOutputfile;
	files filenames;
	filenames.TrialInfoFile = TrialInfoFile;
	filenames.nameOutputfile = nameOutputfile;
	return filenames;
}
