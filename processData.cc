#include "main.ih"

size_t processData(string TrialInfoFile, string filename, size_t timeBefore, string& lock2)
{
  
  string trialInfo;
	ifstream trialInfoFile(TrialInfoFile);
	if (trialInfoFile.is_open())
		getline(trialInfoFile, trialInfo);
	else
	{
		cout << "Unable To Open "<< TrialInfoFile << '\n';
		return 0;
	}  
// 	TrialInfo trialSet();
// 	size_t limit4extraction = 4000;
	TrialInfo trialSet(timeBefore, 4000);
	trialSet.extractInfo(trialInfo);
  trialSet.updateInterp(false);
  string subNum = trialSet.g_subject();  

	ofstream outputfile;
	filename.append("_ALL_medianInterp.asc");
	outputfile.open(filename);
	// write file's header
	outputfile << "pp" << '\t' << "clockTime" << '\t'  << "time" << '\t'
		<< "bin" << '\t' <<  "wordNonWord" << '\t' << "trial" << '\t' 
		<< "item" << '\t' << "psize"  << '\t' << "fix\n" /*<< '\t'  
		<< "RT" << '\t' << "acc\n"*/;  
		
	ifstream interpInfo("tbt_blinksPerSubjectSmaller300.txt");
	Interpdata interpolation; // interpolation.interpolate() set to false unless interpolation file exists
	if (not interpInfo.is_open())
  {
// 		cout << "NO interpolation file "<< interpolationFile << '\n'; 
		cout << "NO interpolation file tbt_blinksPerSubjectSmaller300.txt\n"; 
		cout << "Continue without interpolation information\n\n"; 
  } else {
		interpolation.setDoInterpolation(true); //interpolation.interpolate() set to true
		string line1, line2;
		getline(interpInfo, line1);
		getline(interpInfo, line2);
		if (interpInfo.good())
			interpolation = interpolation.extractInterpData(line1, line2);
	}
	
  while (true)
  {
// use experiment records to open eyetracking datafile for reading
    ifstream eyetrackingFile(subNum.append(".asc"));
    if (! eyetrackingFile.is_open())
    {
      cout << "Unable to open eyetracking datafile " << subNum << '\n'; 
      break;
    }
    else
    {
			cout << "Extracting " << subNum << '\n';
			if (interpolation.interpolate() && (trialSet.g_subject() > interpolation.sub()))
      {
				while (trialSet.g_subject() != interpolation.sub() && not interpInfo.eof())
				{
					string line1, line2;
					getline(interpInfo, line1);
					getline(interpInfo, line2);
					interpolation.setSub("none");
					if (interpInfo.good())
						interpolation = interpolation.extractInterpData(line1, line2);
				} // end while
			} // end if (trialSet.g_subject() > interpolation.sub())
      trialSet.setCurrentTr(0);
      trialSet.resetBinsCounter();
      trialSet.setFix(1);
      
      Dataline eye, preBlink;
      string line;
      
      while (getline(eyetrackingFile, line))
      {
				/* ignore lines which are fix or sac messages - SFIX is used to count fixations */
				while ((line.find("EFIX") != string::npos) || (line.find("SAC") != string::npos))
					getline(eyetrackingFile, line);
				
// 				if (line.find("onsetSoundStim") != string::npos)
				if (line.find(lock2) != string::npos)
					trialSet.updateCurrentTrial(line);
// 				{
// 					cout << 's' << trialSet.g_currentTr() << ' ' << trialSet.g_updateInterp() << '|'; 
// 				}	
				if (trialSet.g_currentTr() == trialSet.g_trialIN())
				{
					if (line.find("TRIAL ENDS") != string::npos)
					{
						trialSet.resetAndUpdate(trialInfoFile);
						// interrupt if the data for another subject has been read
// 						cout << subNum << ' ' << trialSet.g_subject() << '\n';
						if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject())
						{
							eyetrackingFile.close();
							break;
						}
					}
					
					if (line.find("SBLINK") != string::npos)
					{
						
						trialSet = interpolateBlinks(trialInfoFile, eyetrackingFile, outputfile,
																				 trialSet, preBlink, interpolation, lock2);
// 						cout << trialSet.g_updateInterp() << 'i';
						if (trialSet.g_updateInterp())
						{
							// read eyetrackingFile's lines up to the end of the interpolation interval
							if (trialSet.g_subject() == interpolation.sub() && 
								trialSet.g_currentTr() == interpolation.nTrial())
							{
								while (eye.g_time() < interpolation.iEnd())
								{
									getline(eyetrackingFile, line);
									eye.extractData(line);
								}
							}
							// update information about the next interpolation
							string line1, line2;
							getline(interpInfo, line1);
							getline(interpInfo, line2);
							interpolation.setSub("none");
							if (interpInfo.good())
								interpolation = interpolation.extractInterpData(line1, line2);
// 							cout << interpolation.nTrial() << ' ' << trialSet.g_currentTr() << 
// 								' ' << interpolation.sub() << '\n'; 
							trialSet.updateInterp(false);
							getline(eyetrackingFile, line);
						} // end: if (trialSet.g_updateInterp())
					} // end "if (line.find("SBLINK") != string::npos)"

					eye.extractData(line);
					
					if ((eye.isMSG() != true) && (eye.g_time() >= trialSet.g_startExport()) && 
						(eye.g_time() <= trialSet.timeIsUp()))
					{
						if(eye.isValid())
						{
							// participants with no interpolation can go without problems
							if (trialSet.g_subject() != interpolation.sub())
							{
								trialSet.addOneBin();
								writeOut(eye, outputfile, trialSet);
							}
							else
							{  
								if (eye.g_time() <= interpolation.iBegin())
								{
									trialSet.addOneBin();
									writeOut(eye, outputfile, trialSet);
								} // end if-else: if (trialSet.g_subject() != interpolation.sub())
							} // end: if(eye.isValid())
						} // end : if((eye.isMSG() != true) && (eye.g_time() >= (trialSet.g_targetOnset() - timeBefore)) && (eye.g_time() <= trialSet.timeIsUp()))
					} // end of "if (!eye.isMSG && (eye.g_time() >= trialSet.g_targetOnset()))"
					
					if ((line.find("SFIX") != string::npos) && 
						(eye.g_time() >= trialSet.g_startExport()) && 
						(eye.g_time() <= trialSet.timeIsUp()))
						trialSet.setFix(trialSet.g_Fix() + 1);
							
					preBlink = eye;
					
					// there are cases when interpolation data are present for trials which should not be interpolated.
					// For example: a participant blinks multiple times within a trial. The majority of the blinks 
					// are initially shorter than 300 ms but then there is one that is longer than 300ms. The trial 
					// should be excluded, but the interpolation data are still there. Interpolation data for that
					// trial should be ignored if the trial has been excluded.
// 					if (interpolation.interpolate() && 
// 						trialSet.g_subject() == interpolation.sub() && 
// 						trialSet.g_currentTr() > interpolation.nTrial() &&
// 						not interpInfo.eof() )
					if (trialSet.g_subject() == interpolation.sub() && 
						trialSet.g_currentTr() > interpolation.nTrial() &&
						not interpInfo.eof() )
					{
						cout << line;
						cout << "skipping lines\n";
						string line1, line2;
						getline(interpInfo, line1);
						getline(interpInfo, line2);
						interpolation.setSub("none"); // why initialization to none?
						if (interpInfo.good())
							interpolation = interpolation.extractInterpData(line1, line2);
						
					}// if (interpolation.interpolate() && ...
				} // end of "if (iTrial == trialSet.g_trialIN())"
      } // end of "while(getline(eyetrackingFile, line))"
    } // end "if(! eyetrackingFile.is_open()") 
    
    eyetrackingFile.close();
    subNum = trialSet.g_subject();  

    if (trialInfoFile.eof())
      break;
    
	} // end "while(true)"
  
  trialInfoFile.close();
  outputfile.close();
  
  return 1;
}