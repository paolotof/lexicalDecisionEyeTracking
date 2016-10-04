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

// size_t checkBlinksAndGetInterpBounds(string filename, string nameOutputfile, string& lock2)
size_t checkBlinksAndGetInterpBounds(string filename, string nameOutputfile, string& lock2)
{

	ifstream trialInfoFile(filename);
	if (not trialInfoFile.is_open())
		return 0;
	
  filename.replace(filename.find(".txt"), 4, "_noBlinks.txt");
// 	filename = nameOutputfile + '_' + filename;
  // file containing blinks longer than 300 ms
  ofstream longBlinksReport;
  longBlinksReport.open(nameOutputfile.append("blinksPerSubject.txt")); 
	
	// file containing blinks shorter than 300 ms
  ofstream shortBlinksReport;
  shortBlinksReport.open(nameOutputfile.replace(nameOutputfile.find(".txt"), 4,
    "Smaller300.txt"));  
	
  // file containing trials to be included without blinks
  ofstream updatedTrialInfoFile;
  updatedTrialInfoFile.open(filename);
  
	string trialInfo;
  TrialInfo trialSet;
  while (getline(trialInfoFile, trialInfo))
  {
    trialSet.extractInfo(trialInfo);
    string subNum = trialSet.g_subject();  
    ifstream eyetrackingFile(subNum.append(".asc"));
    if (not eyetrackingFile.is_open())
    {
      cout << "Unable to open eyetracking datafile " << subNum << '\n';
      break;
    }
    else
    {
      cout << "Blinks and interpolation " << trialSet.g_subject() << '\n';
      trialSet.setCurrentTr(0);
      bool includeTrial = true;
      vector<double> tVect;
      vector<double> xVect;
      vector<double> yVect;
      vector<double> pVect;
      
      size_t lines2include = 25; // lines to include for interpolation
      double medianT;
      double medianX;
      double medianY;
      double medianP;
             
      string line;
      Dataline eye;
      //size_t onsetWord = 0;
      double blinkDuration = 0;
      
      while (getline(eyetrackingFile, line))
      {
			// BEGINNING OF WORD SOUND
// 				if (line.find("onsetSoundStim") != string::npos)
				if (line.find(lock2) != string::npos)	
					trialSet.updateCurrentTrial(line);

			// TRIAL ENDS - update trial information/reset arrays
				if ((line.find("TRIAL ENDS") != string::npos) && (trialSet.g_currentTr() == trialSet.g_trialIN()))
				{
					
					if (includeTrial)
						updatedTrialInfoFile << trialInfo << '\n';
					includeTrial = true; // reset include trials
					blinkDuration = 0;
					getline(trialInfoFile, trialInfo); 
					trialSet.extractInfo(trialInfo);
					// if the current subject does not have trials that should be 
					// considered anymore stop with this file and get to the next one
					if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject())
					{
// 						cout << '\n' << subNum << " processing ends at trial " << trialSet.g_currentTr() << '\n';
// 						cout << " trials=" << trialSet.g_currentTr() << '\n';
						eyetrackingFile.close();
						break;
					} // if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject())
					
					vector<double>().swap(tVect);
					vector<double>().swap(xVect);
					vector<double>().swap(yVect);
					vector<double>().swap(pVect);
					
				} // if ((line.find("TRIAL ENDS") != string::npos) && (trialSet.g_currentTr() == trialSet.g_trialIN()))

			// BLINK STARTS - get median of previously stored values
				if ((line.find("SBLINK") != string::npos) && (trialSet.g_currentTr() == trialSet.g_trialIN()))
				{
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
				if ((line.find("EBLINK") != string::npos) && (trialSet.g_currentTr() == trialSet.g_trialIN()))
				{
					istringstream linedata(line); // extract time information from the datafile
					string msgType, side;
					double blinkBegin, blinkEnd;
					linedata >> msgType >> side >> blinkBegin >> blinkEnd;
					
				
					// 1) Does blink happens while target word is played?
					// if it 1.a) does not - ignore, 
					//       1.b) does - check the length of the blink from onset of target word. 
// 					if ((blinkEnd >= trialSet.g_targetOnset()) && (blinkBegin <= trialSet.timeIsUp()))
// 					{
// 						
// 						blinkDuration = blinkEnd - blinkBegin;
// 						
// 						if (blinkEnd >= trialSet.timeIsUp())
// 							blinkDuration = trialSet.timeIsUp() - blinkBegin;
// 						if (blinkBegin <= trialSet.g_targetOnset())
// 							blinkDuration = blinkEnd - trialSet.g_targetOnset();
						
					if ((blinkEnd >= trialSet.g_startExport()) && (blinkBegin <= trialSet.timeIsUp()))
					{
						
						blinkDuration = blinkEnd - blinkBegin;
						
						if (blinkEnd >= trialSet.timeIsUp())
							blinkDuration = trialSet.timeIsUp() - blinkBegin;
						if (blinkBegin <= trialSet.g_startExport())
							blinkDuration = blinkEnd - trialSet.g_startExport();
						// 2) If length of the blink from onset word is 
						// 	2.b > 300 ms - exclude 
						//    2.b < 300 ms - interpolate
						
						bool exportData;
// 							cout << ' ' << trialSet.g_currentTr(); //<< ' ' << blinkDuration ;
						if (blinkDuration < 300)
						{
							includeTrial = true;
							
							if (blinkDuration > 20)	
							{
// 									cout << '.'; // add a dot to filename for each blink needing interpolation with median
								exportData = true;
								// get other 25 lines of data to get the median of
								for (size_t lines2skip = 0; lines2skip < lines2include; lines2skip++) 
								{
									getline(eyetrackingFile, line);
									if (line.find("TRIAL ENDS") != string::npos)
									{
										// interrupt all extrapolation for current blink
										vector<double>().swap(tVect);
										vector<double>().swap(xVect);
										vector<double>().swap(yVect);
										vector<double>().swap(pVect);
										// do all the operation for 'trials2beIncluded' file update, since other blinks 
										// in this current trial should be included
										updatedTrialInfoFile << trialInfo << '\n';
										getline(trialInfoFile, trialInfo); 
										trialSet.extractInfo(trialInfo);
										exportData = false; // interrupt interpolation for this specific blink
										
										/* this creates problem because _blinksPerSubjectSmaller300 has different trials
										*  then trials2beIncluded_x_noBlinks file. Therefore it's preferable to read the 
										*  whole eyetracking file
										*/
										if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject())
										{
// 												cout << '\n' << subNum << " processing ends at trial " << trialSet.g_currentTr() << '\n';
											eyetrackingFile.close();
											break;
										} // if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject())
										break; // breaks the for loop
									} // if (line.find("TRIAL ENDS") != string::npos)
									
									if (line.find("SBLINK") != string::npos) // in case a blink happens before 25 lines
									{
										// fish a new line and break
										getline(eyetrackingFile, line);
										exportData = false;
										break;
									} //if (line.find("SBLINK") != string::npos)
									
									eye.extractData(line);
									if (not eye.isMSG() && eye.isValid())
									{
										tVect.push_back(eye.g_time());
										xVect.push_back(eye.g_xpos());
										yVect.push_back(eye.g_ypos());
										pVect.push_back(eye.g_psize());
									} // if (not eye.isMSG() && eye.isValid())
								} // for (size_t lines2skip = 0; lines2skip < lines2include; lines2skip++) 
								// for loop to collect data after end of the blink
								
								if (exportData) // Export data only if 'TRIAL ENDS' message wasn't encountered
								{
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
									// ? why divided by 4?
									vector<double>().swap(tVect);
									vector<double>().swap(xVect);
									vector<double>().swap(yVect);
									vector<double>().swap(pVect);
								} // end if (exportData) 
							} // end if-else (blinkDuration < 20)
						} // end if-else (blinkDuration > 300)	    
						else
						{
// 								cout << '-';
							longBlinksReport << subNum << ' ' << trialSet.g_currentTr() << ' ' << blinkDuration << '\n'; 
							includeTrial = false; 
							// Skip remaining lines of this current trial. Go to the next trial searching for 'TRIAL ENDS' 
							while (getline(eyetrackingFile, line))
							{
								//cout << line << '\n';
								if (line.find("TRIAL ENDS") != string::npos)
								{
									blinkDuration = 0;
									getline(trialInfoFile, trialInfo); 
									trialSet.extractInfo(trialInfo);
									vector<double>().swap(tVect);
									vector<double>().swap(xVect);
									vector<double>().swap(yVect);
									vector<double>().swap(pVect);
									break;
								} // if (line.find("TRIAL ENDS") != string::npos)
							} // end 'while (getline(eyetrackingFile, line))'
						} // end if-else - if (blinkDuration < 300)
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
					if (tVect.size() <= lines2include)
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

    if (trialInfoFile.eof())
      break;
    
  } // end "while(getline(trialInfoFile, trialInfo))"
  
  trialInfoFile.close();
  updatedTrialInfoFile.close();
  longBlinksReport.close();
  shortBlinksReport.close();
  
// 	return "blinks processed";
	return 0;
}