#include "trialinfo.ih"

void TrialInfo::setTargetLimits(string& line)
{
  istringstream linedata(line);
  string msgType, trialNum, starts;
  size_t trialStarts, wordStarts; 
// 	linedata >> msgType >> wordStarts;
	linedata >> msgType >> trialStarts >> trialNum >> starts >> wordStarts;
	TrialInfo::setOnsetTarget(wordStarts); 
// 	TrialInfo::setTimeIsUp(TrialInfo::g_targetOnset() +
// 		TrialInfo::limit4extraction());
	TrialInfo::setTimeIsUp(wordStarts +
		TrialInfo::limit4extraction());
// 	TrialInfo::setStartExport(TrialInfo::g_targetOnset() -
// 		TrialInfo::g_timeBefore());
// 	TrialInfo::setStartExport(TrialInfo::g_targetOnset() -
// 		trialStarts);
// 	TrialInfo::setTimeBefore(trialStarts);
// 	TrialInfo::setStartExport(TrialInfo::g_targetOnset() -
// 		TrialInfo::g_timeBefore());
	TrialInfo::setStartExport(trialStarts);
	
}
