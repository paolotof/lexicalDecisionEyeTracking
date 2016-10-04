#include "trialinfo.ih"

void TrialInfo::computeTargetLimits(string& line)
{
  istringstream linedata(line);
  string msgType;
  size_t wordStarts; 
	linedata >> msgType >> wordStarts;
	TrialInfo::setOnsetTarget(wordStarts); 
	TrialInfo::setTimeIsUp(TrialInfo::g_targetOnset() +
		TrialInfo::limit4extraction());
	TrialInfo::setStartExport(TrialInfo::g_targetOnset() -
		TrialInfo::g_timeBefore());
}
