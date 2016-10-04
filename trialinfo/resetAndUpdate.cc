#include "trialinfo.ih"

//TrialInfo TrialInfo::resetAndUpdate(ifstream& trialInfoFile)
void TrialInfo::resetAndUpdate(ifstream& trialInfoFile)
{
  TrialInfo::resetBinsCounter(); // in case it hasn't been reset before
  string trialInfo;
  getline(trialInfoFile, trialInfo); 
  TrialInfo::setFix(1);
  TrialInfo::extractInfo(trialInfo);
//  return(trialSet);
} 
