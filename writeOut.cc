#include "main.ih"

//TrialInfo writeOut(Dataline eye, ofstream& outputfile, TrialInfo trialSet)
void writeOut(Dataline eye, ofstream& outputfile, TrialInfo trialSet)
{
  double timeDif = eye.g_time() - trialSet.g_targetOnset();
// conversion to size_t with static_cast is for rounding
  outputfile << trialSet.g_subject(); 
	size_t old_precision = outputfile.precision();
	outputfile.precision(10);
	outputfile << '\t' << static_cast<size_t>(eye.g_time());
	outputfile.precision(old_precision);
// 	outputfile << '\t' << static_cast<size_t>(timeDif)
	outputfile << '\t' << timeDif
		<< '\t' << static_cast<int>(timeDif / 4)
// 		<< '\t' << trialSet.totalBins()
		<< '\t' << trialSet.g_condition() 
		<< '\t' << trialSet.g_trialIN() 
    << '\t' << trialSet.g_target() 
// 		we also got rid of x and y positions
    << '\t' << eye.g_psize()
		<< '\t' << trialSet.g_Fix()
// 		<< '\t' << trialSet.g_RT()
// 		<< '\t' << trialSet.g_acc()
		<< '\n';
      
}
