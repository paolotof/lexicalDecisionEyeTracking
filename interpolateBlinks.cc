#include "main.ih"

/* 
 * 
 * determine blinks length and linear interpolate blink 
 * shorter than 20 ms with after and before values and 
 * use the median for blinks between 20 and 300 ms long
 * 
 * 
 */

// TrialInfo interpolateBlinks(ifstream& trialInfoFile, ifstream& eyetrackingFile, ofstream& outputfile, 
// 														TrialInfo trialSet, Dataline preBlink, Interpdata interpolation, 
// 														size_t timeBefore, string& lock2)
TrialInfo interpolateBlinks(ifstream& trialInfoFile, ifstream& eyetrackingFile, 
														ofstream& outputfile, TrialInfo trialSet, Dataline preBlink, 
														Interpdata interpolation, string& lock2)
{ 
  string line;
	// loop until end of blink message is found
  while (getline(eyetrackingFile, line)) {
    if (line.find("EBLINK") == string::npos)
      break; // stop "while (getline(eyetrackingFile, line))"
   
    if (line.find("TRIAL ENDS") != string::npos) {
      trialSet.resetAndUpdate(trialInfoFile);
      return(trialSet); }

//     if (line.find("onsetSoundStim") != string::npos)  
		if (line.find(lock2) != string::npos) {
      trialSet.updateCurrentTrial(line);
      return(trialSet); }
  } // end "while (getline(eyetrackingFile, line))"
// 	cout << line << '\n';
  Dataline postBlink;
// search first data line after the end-of-blink message
  while (getline(eyetrackingFile, line)) {
    postBlink.extractData(line);
    if (postBlink.isValid() && (not postBlink.isMSG()))
      break;
    
    if (line.find("TRIAL ENDS") != string::npos) {
      trialSet.resetAndUpdate(trialInfoFile);
      return(trialSet); }
//     if (line.find("onsetSoundStim") != string::npos)  
		if (line.find(lock2) != string::npos) {
      trialSet.updateCurrentTrial(line);
      return(trialSet); }
  } // end "while (true)"
	
// 	cout << line << '\n';
	// Interpolate if necessary
  size_t blinkDur = postBlink.g_time() - preBlink.g_time();
	
	//cout << blinkDur;// <<'\n';
	
	// this does not add anything
// 	if (blinkDur > 300)
// 		return(trialSet);
	
	// long blink	  
  if ((blinkDur > 20) && (interpolation.sub() ==  trialSet.g_subject()) && 
    (trialSet.g_currentTr() == interpolation.nTrial())) {
    if ((preBlink.g_time() <= interpolation.iEnd()) && (postBlink.g_time() >= interpolation.iBegin())) {
			trialSet.updateInterp(interpolateLongBlinks(outputfile, trialSet, interpolation));
			if (interpolation.iBegin() < trialSet.g_targetOnset())
				trialSet.updateBinCount((interpolation.iEnd() - trialSet.g_targetOnset()) / 4);
      else
				trialSet.updateBinCount((interpolation.iEnd() - interpolation.iBegin()) / 4); }
  }
  // short blink		  
  if (blinkDur >= 0 && blinkDur <= 20) {
    size_t lines2interpolate = blinkDur / 4;
		interpolateShortBlinks(outputfile, trialSet, preBlink, postBlink, lines2interpolate);
		trialSet.updateBinCount(lines2interpolate); }
    
  return(trialSet);
}
