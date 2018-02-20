#include "main.ih"

size_t newProcessData(files filenames, string filename, 
									 size_t timeBefore, string& lock2, 
									 size_t limit4extraction, string& filePrefix){
	
	ifstream interpInfo(filenames.nameInterpolationFile);
	Interpdata interpolation; // interpolation.interpolate() set to false unless interpolation file exists
// with the new version we read the interpolation file new everytime until we find the current participant
// and trial, if we do not find it we proceed without interpolation
	if (not interpInfo.is_open()) {
		cout << "NO interpolation file " << filenames.nameInterpolationFile << "\n"; 
		cout << "Continue without interpolation information\n\n"; 
		interpInfo.close(); } 
	
// start data processing
  string trialInfo;
	ifstream trialInfoFile(filenames.TrialInfoFile);
	if (trialInfoFile.is_open())
		getline(trialInfoFile, trialInfo);
	else {
		cout << "Unable To Open "<< filenames.TrialInfoFile << '\n';
		return 0; }  
	TrialInfo trialSet(timeBefore, limit4extraction);
	trialSet.extractInfo(trialInfo);
  trialSet.updateInterp(false);
	string subNum = trialSet.g_subject();  
	ofstream outputfile;
	filename.append("_ALL_medianInterp.asc");
	outputfile.open(filename);
// write file's header
	outputfile << "pp" << '\t' << "clockTime" << '\t'  << "time" << '\t'
// 		<<  "wordNonWord" << '\t' 
		<< "trial" << '\t' 
		// << "item" << '\t' // removed item because we have trial number already, file is smaller
		<< "psize"  << '\t' << "fix\n";  
		
  while (true)
  {
// use experiment records to open eyetracking datafile for reading
    ifstream eyetrackingFile(subNum.append(".asc"));
    if (! eyetrackingFile.is_open()) {
      cout << "Unable to open eyetracking datafile " << subNum << '\n'; 
      break;
    } else {
			cout << "Extracting " << subNum << '\n';
			trialSet.setCurrentTr(0);
      trialSet.resetBinsCounter();
      trialSet.setFix(1);
      Dataline eye, preBlink;
      string line;
      while (getline(eyetrackingFile, line)){
				/* ignore lines which are fix or sac messages - SFIX is used to count fixations */
				while ((line.find("EFIX") != string::npos) || (line.find("SAC") != string::npos))
					getline(eyetrackingFile, line);
// find line stating the trial BEGINS				
				if (line.find(lock2) != string::npos){
					trialSet.updateCurrentTrial(line);
					// interpolation is false if the current trial does not have to be interpolated.
					if (trialSet.g_currentTr() == trialSet.g_trialIN()){
// 						interpolation = currentInterpInfo(trialSet, filenames); // MIND: in this way
					// the reading of the interpolationInfo file does not proceed here, but within
					// the function call, it needs to be updated here too. Let's not use the function
						// start from the beginning closing and opening the file
// 						outputfile << " interp " << trialSet.g_currentTr() << ' ' << trialSet.g_trialIN() << '\n';
						interpInfo.close(); 
						interpInfo.open(filenames.nameInterpolationFile);
						// read first two lines to create an antecedent for the match operation
						// and find the interpolation information for current participant and trial
						string line1, line2;
// 						getline(interpInfo, line1); // reads 2 lines at the time because paired
// 						getline(interpInfo, line2);
// 						interpolation.setSub("none");
// 						if (interpInfo.good()){
// 							interpolation = interpolation.extractInterpData(line1, line2); 
// 							interpolation.setDoInterpolation(true); }
// 							while (trialSet.g_subject() != interpolation.sub()
// 								&& trialSet.g_currentTr() != interpolation.nTrial()){
						while (true){
							if (interpInfo.eof())
								break;
							getline(interpInfo, line1); // reads 2 lines at the time because paired
							getline(interpInfo, line2);
							interpolation.setSub("none");
							if (interpInfo.good()){
								interpolation = interpolation.extractInterpData(line1, line2); 
								interpolation.setDoInterpolation(true); } 
							if (trialSet.g_subject() == interpolation.sub()
								&& trialSet.g_currentTr() == interpolation.nTrial()
								&& interpolation.iEnd() > eye.g_time() // sometimes the interpolation info is before the trial starts
							)
								break;
						}
// 						outputfile << line2 << '\n';		
					} // END: if (trialSet.g_currentTr() == trialSet.g_trialIN()){
				} // END: if (line.find(lock2) != string::npos){
				
				if (trialSet.g_currentTr() == trialSet.g_trialIN()){
// "TRIAL ENDS"					
					if (line.find("TRIAL ENDS") != string::npos){
						trialSet.resetAndUpdate(trialInfoFile);
						// interrupt if the data for another subject has been read
// 						cout << subNum << ' ' << trialSet.g_subject() << '\n';
						if (subNum.substr(0, subNum.find(".asc")) != trialSet.g_subject()){
							eyetrackingFile.close();
							break; } }
// "SBLINK"
					if (line.find("SBLINK") != string::npos){
						trialSet = interpolateBlinks(trialInfoFile, eyetrackingFile, outputfile,
																				 trialSet, preBlink, interpolation, lock2);
						// interpolation information must be updated too:
						// in the cases in which interpolation information are before the export
						// interval otherwise the interpolation information sticks with the
						// time interval before the export period
						if (trialSet.g_subject() == interpolation.sub()  
							&& trialSet.g_currentTr() == interpolation.nTrial()
							&& (preBlink.g_time() > interpolation.iEnd()))//{
							trialSet.updateInterp(true);
// 							outputfile << "should be updating interpolation" << '\n';
// 						}
						if (trialSet.g_updateInterp()){
							// read eyetrackingFile's lines up to the end of the current interpolation interval
							if (trialSet.g_subject() == interpolation.sub() && 
								trialSet.g_currentTr() == interpolation.nTrial()){
								while (eye.g_time() < interpolation.iEnd()){
// 								while (eye.g_time() <= interpolation.iEnd()){ // or does it skip a line like this?
									getline(eyetrackingFile, line); // this does not seem to make a difference
									eye.extractData(line); } }
							// update information about the next interpolation
							string line1, line2;
							getline(interpInfo, line1);
							getline(interpInfo, line2); 
// it is easier to look what's happening printing the output to file than to screen, 
// therefore the interp info is printed after the intrepolation itself							
// 							outputfile << line1 << '\n';
// 							outputfile << line2 << '\n';
							interpolation.setSub("none"); // this is here if something goes wrong with extractInterpData
							if (interpInfo.good())
								interpolation = interpolation.extractInterpData(line1, line2);
// 							outputfile << interpolation.nTrial() << ' ' << trialSet.g_currentTr() 
// 								<< ' ' << interpolation.sub() << ' ' << trialSet.g_subject() << '\n'; 
							trialSet.updateInterp(false);
							getline(eyetrackingFile, line); // already done, why again?
							// should this also look until it does not find a match?
							
						} // end: if (trialSet.g_updateInterp())
					} // end "if (line.find("SBLINK") != string::npos)"
// this was:
// extract data and write out data if possible
// but became:
//					 and write out data if possible
// because in some cases we need to update the file with interpolation information before the trial starts.
// the neatest solution would be to remove interpolation information for the time interval which should not 
// be included in the extraction interval
					// for now we put it back as it was...
					eye.extractData(line);
					if ((eye.isMSG() != true) && (eye.isValid()) 
						&& (eye.g_time() >= trialSet.g_startExport()) 
						&& (eye.g_time() <= trialSet.timeIsUp())) {
							// participants with no interpolation can go without problems
						if (trialSet.g_subject() != interpolation.sub()) {
							trialSet.addOneBin();
							writeOut(eye, outputfile, trialSet);
						} else {  
							// if time outside interpolation range write out, 
							// otherwise interpolate, why? you already interpolated
							if (eye.g_time() <= interpolation.iBegin() 
								|| eye.g_time() > interpolation.iEnd()) {
								trialSet.addOneBin();
								writeOut(eye, outputfile, trialSet); 
							} /*else {
								trialSet = interpolateBlinks(trialInfoFile, eyetrackingFile, outputfile,
																						 trialSet, preBlink, interpolation, lock2);
								
								// make sure current eyeData are sinked with what has been exported
	// 						cout << trialSet.g_updateInterp() << 'i';
								if (trialSet.g_updateInterp()){
									
									// read eyetrackingFile's lines up to the end of the current interpolation interval
									if (trialSet.g_subject() == interpolation.sub() && 
										trialSet.g_currentTr() == interpolation.nTrial()){
										while (eye.g_time() < interpolation.iEnd()){
											getline(eyetrackingFile, line);
											eye.extractData(line); } }
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
							} // END: "if (eye.g_time() <= interpolation.iBegin() ...*/
						} // END: (trialSet.g_subject() != interpolation.sub())
					} //END: of "if ((eye.isMSG() != true) && (eye.isValid()) ...."
					
					// extract data and write out data if possible
					// for now we put it back as it was
// 					eye.extractData(line);
					
					if ((line.find("SFIX") != string::npos) 
						&& (eye.g_time() >= trialSet.g_startExport()) 
						&& (eye.g_time() <= trialSet.timeIsUp()))
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
					// ??? IS THIS STILL NECESSARY ??? 
// 					if (trialSet.g_subject() == interpolation.sub() && 
// 						trialSet.g_currentTr() > interpolation.nTrial() &&
// 						not interpInfo.eof() ) {
// // 						cout << line << '\n';
// // 						cout << trialSet.g_subject() << " . " << interpolation.sub() << '\n';
// 						cout << "skipping lines " << trialSet.g_subject() << ' '
// 						  << trialSet.g_currentTr() << ' ' << interpolation.nTrial() << '\n';
// 						string line1, line2;
// 						getline(interpInfo, line1);
// 						getline(interpInfo, line2);
// 						interpolation.setSub("none"); // why initialization to none?
// 						if (interpInfo.good())
// 							interpolation = interpolation.extractInterpData(line1, line2);
// 					}// if (interpolation.interpolate() && ...
				} // end of "if (iTrial == trialSet.g_trialIN())"
				eye.extractData(line);	// maybe this is not necessary
      } // end of "while(getline(eyetrackingFile, line))"
    } // end "if(! eyetrackingFile.is_open()") 
    
    eyetrackingFile.close();
    subNum = trialSet.g_subject();  

    if (trialInfoFile.eof())
      break;
    
	} // end "while(true)"
  
  trialInfoFile.close();
  outputfile.close();
	interpInfo.close();
	
  return 1;
// 	return(filename);
}
