#include "trialinfo.ih"

void TrialInfo::extractInfo(string& line)
{
  
  istringstream linedata(line);
  
  string Subject, Target;

// 	size_t Trial, RTtime, accuracy;
	size_t Trial, Condition;
	double RT;
	
// 	linedata >> Subject >> Trial >> Target >> Condition >> RTtime >> accuracy; 
	linedata >> Subject >> Trial >> Target >> Condition >> RT; 
	
// 	cout << line << ' ' ;
// 	cout << Trial << ' ' << Condition<< ' ' ; 
// 	cout << RT << '\n';
	
  TrialInfo::setSubject(Subject);
  TrialInfo::setTrialIN(Trial);
	TrialInfo::setTarget(Target);
  TrialInfo::setCondition(Condition);
//   TrialInfo::setRT(RTtime);
//   TrialInfo::setAccuracy(accuracy);
	
	// 1 - transform reaction times in ms
	// 2 - add 1000 ms to extend the amount of data exported
	TrialInfo::setLimit4extraction(static_cast<size_t>(RT * 1000));
	
// 	cout << RT  << ' ' << (RT * 1000)  << ' '<< static_cast<size_t>(RT * 1000) << ' ' << TrialInfo::limit4extraction();
// 	cout << TrialInfo::limit4extraction();
  TrialInfo::setBin(0); // reset counter
  TrialInfo::setCurrentTr(TrialInfo::g_currentTr());
  
}