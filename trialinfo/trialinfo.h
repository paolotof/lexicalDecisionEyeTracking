#ifndef INCLUDED_TRIALINFO_
#define INCLUDED_TRIALINFO_

#include <string>
#include <sstream> 
#include <fstream>

using namespace std;

class TrialInfo
{
  string d_subject;
  size_t d_trialIN;	 
  size_t d_currentTr = 0;  
  string d_target;
  size_t d_condition;
  size_t d_onsetTarget;
  size_t d_RT;
	size_t d_acc;
  string d_subCond;
  size_t d_iBin;
  size_t d_binCount = 0;
  size_t d_fixCount = 1; // always starts with 1 and then update
  bool d_updateInterp = false; // 26-4-16: given a default so that it is valid if running people without interpolation 
  size_t d_limit4extraction = 4000;// we could make this to be either a number or a string as i.e. (TRIAL ENDS)
  size_t d_timeBefore = 1000;
  size_t d_startExport;
	size_t d_timeIsUp;
  
  public:
    TrialInfo();
		TrialInfo(size_t timeBefore);
//		TrialInfo(size_t limit4extraction); overloading problem
		TrialInfo(size_t timeBefore, size_t d_limit4extraction);
		
// SETTERS  
// we leave these in because otherwise we get problems at compilation. Functions should remove totally
		void setAccuracy(size_t accuracy);
    void setRT(size_t RT);
//
    void setBin(size_t iBin);
    void setCondition(size_t Condition);
    void setCurrentTr(size_t Trial);
    void setFix(size_t numFixes);
		void setLimit4extraction(size_t limit4extraction);
    void setOnsetTarget(size_t onset);
		void setStartExport(size_t startExport);
    void setSubject(string Subject);
    void setTarget(string Target);
    void setTimeIsUp(size_t limit2export);
		void setTimeBefore(size_t timeBefore);
    void setTrialIN(size_t Trial);
    void addOneBin();
    void updateBinCount(size_t nBins);
    void setTargetLimits(string& line);
    void extractInfo(string& line);
    void resetAndUpdate(ifstream& trialInfoFile);
    void resetBinsCounter();
    void updateCurrentTrial(string& line);
    void updateInterp(bool value); 
// GETTERS    
    // there is a g in front of the name because it's then easier to recognize the file names of set versus get functions.
    size_t g_Bin() const;
    size_t g_condition() const;
    size_t g_currentTr() const;
    size_t g_Fix() const;
// we leave these in because otherwise we get problems at compilation. Functions should remove totally
		size_t g_RT() const;
		size_t g_acc() const;
//
		size_t g_startExport() const;
    string g_subCond() const;
    string g_subject() const;
    string g_target() const;
    size_t g_targetOnset() const;
    size_t g_timeBefore() const;
    size_t g_trialIN() const;
    bool   g_updateInterp() const;
    size_t limit4extraction() const; 
    size_t timeIsUp() const;
    size_t totalBins() const;
		
  private:
};
 
#endif
