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
														ofstream& outputfile, TrialInfo trialSet, Dataline preBlink, string& lock2){ 
  string line;
	// loop until end of blink message is found
  while (getline(eyetrackingFile, line)) {
    if (line.find("EBLINK") == string::npos)
      break; // stop "while (getline(eyetrackingFile, line))"
   
    if (line.find("TRIAL ENDS") != string::npos) {
      trialSet.resetAndUpdate(trialInfoFile);
      return(trialSet); }

		if (line.find(lock2) != string::npos) {
      trialSet.updateCurrentTrial(line);
      return(trialSet); }
  } // end "while (getline(eyetrackingFile, line))"
  Dataline postBlink;
// search first data line after the end-of-blink message
  while (getline(eyetrackingFile, line)) {
    postBlink.extractData(line);
    if (postBlink.isValid() && (not postBlink.isMSG()))
      break;
    if (line.find("TRIAL ENDS") != string::npos) {
      trialSet.resetAndUpdate(trialInfoFile);
      return(trialSet); }
		if (line.find(lock2) != string::npos) {
      trialSet.updateCurrentTrial(line);
      return(trialSet); }
  } // end "while (true)"
	
	// Interpolate if blink is shorter than 20ms
  size_t blinkDur = postBlink.g_time() - preBlink.g_time();
  if (blinkDur >= 0 && blinkDur <= 20) {
    size_t lines2interpolate = blinkDur / 4;
		interpolateShortBlinks(outputfile, trialSet, preBlink, postBlink, lines2interpolate);
		trialSet.updateBinCount(lines2interpolate); }
    
  return(trialSet);
}
