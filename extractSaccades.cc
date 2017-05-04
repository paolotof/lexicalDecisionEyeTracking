#include "main.ih"

size_t extractSaccades(string TrialInfoFile, string filename, string& lock2)
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
	TrialInfo trialSet;
	trialSet.extractInfo(trialInfo);
  trialSet.updateInterp(false);
  string subNum = trialSet.g_subject();  

	ofstream outputfile;
	filename.append("_saccades.asc");
	outputfile.open(filename);
	// write file's header
	outputfile << "pp" << '\t' << "beginT" << '\t'  << "endT" << '\t'
		<< "onsetT" << '\t' 
		<< "beginX" << '\t' << "endX" << '\t' 
		<< "beginY" << '\t' << "endY" << '\t'
		<<  "wordNonWord" << '\t' << "trial" << '\t' 
		<< "item" << '\t' << "totalVisualAngle"  << '\t' << "peakVelocity\n";  
		
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
			cout << "SACCADES " << subNum << '\n';
      trialSet.setCurrentTr(0);
      trialSet.resetBinsCounter();
      trialSet.setFix(1);
      string line;
      
      while (getline(eyetrackingFile, line))
      {
// 				//string.find() returns string::npos when it does not find a match
				if (line.find(lock2) != string::npos)
					trialSet.updateCurrentTrial(line);
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
// 
					// NOTE: I was busy here
					if (line.find("ESACC") != string::npos) 
					{
						string ESACC, whichEye; 
						size_t startTime, endTime, dur;
						double startX, startY, endX, endY, totalVisualAngle, peakVelocity;
						istringstream linedata(line);
						linedata >> ESACC >> whichEye >> startTime >> endTime >> dur >>
							startX >> startY >> endX >> endY >> 
							totalVisualAngle >> peakVelocity;
						if( (startTime >= trialSet.g_startExport()) && 
							(endTime <= trialSet.timeIsUp()) )
						{	
							outputfile << trialSet.g_subject() << '\t' << startTime << '\t' 
								<< endTime << '\t' 
								<< trialSet.g_targetOnset() << '\t' 
								<< startX << '\t' << endX << '\t' 
								<< startY << '\t' << endY << '\t' << trialSet.g_condition() << '\t' 
								<< trialSet.g_currentTr() << '\t' << trialSet.g_target() << '\t' 
								<< totalVisualAngle << '\t' << peakVelocity << '\n';
						}
					} // end "if (line.find("SBLINK") != string::npos)"
// 
// 					
// 					if ((eye.isMSG() != true) && (eye.g_time() >= trialSet.g_startExport()) && 
// 						(eye.g_time() <= trialSet.timeIsUp()))
// 					{
// 						if(eye.isValid())
// 						{
// 							// participants with no interpolation can go without problems
// 							if (trialSet.g_subject() != interpolation.sub())
// 							{
// 								trialSet.addOneBin();
// 								writeOut(eye, outputfile, trialSet);
// 							}
// 							else
// 							{  
// 								if (eye.g_time() <= interpolation.iBegin())
// 								{
// 									trialSet.addOneBin();
// 									writeOut(eye, outputfile, trialSet);
// 								} // end if-else: if (trialSet.g_subject() != interpolation.sub())
// 							} // end: if(eye.isValid())
// 						} // end : if((eye.isMSG() != true) && (eye.g_time() >= (trialSet.g_targetOnset() - timeBefore)) && (eye.g_time() <= trialSet.timeIsUp()))
// 					} // end of "if (!eye.isMSG && (eye.g_time() >= trialSet.g_targetOnset()))"
// 					
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
