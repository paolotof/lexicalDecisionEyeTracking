#include "trialinfo.ih"

void TrialInfo::setTargetLimits(string& line)
{
  istringstream linedata(line);
  string msgType, trialNum, starts;
  size_t trialStarts, wordStarts; 
	linedata >> msgType >> trialStarts >> trialNum >> starts >> wordStarts;
	TrialInfo::setOnsetTarget(wordStarts); 
// 	TrialInfo::setStartExport(trialStarts);
	// I want the trials to start n ms before the onset of the target
	TrialInfo::setStartExport(wordStarts - TrialInfo::g_timeBefore());
	TrialInfo::setTimeIsUp(wordStarts +
		TrialInfo::limit4extraction()); // Is limit4extraction updated?
// e.g. => TrialInfo::setLimit4extraction(static_cast<size_t>(RT * 1000) + 1000);
// 	 or is it the one from the previous trial?
	// it is updated through TrialInfo::extractInfo when a trial to be included
	// finishes
	
	
}
