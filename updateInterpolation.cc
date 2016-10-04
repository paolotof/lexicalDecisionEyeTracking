#include "main.ih"
// there are cases when interpolation data are present for trials which should not be interpolated.
// For example: a participant blinks multiple times within a trial. The majority of the blinks 
// are initially shorter than 300 ms but then there is one that is longer than 300ms. The trial 
// should be excluded, but the interpolation data are still there. Interpolation data for that
// trial should be ignored if the trial has been excluded.

Interpdata updateInterpolation(Interpdata interpolation, TrialInfo trialSet, 
															 string& line, ifstream& interpInfo)					
{			
	cout << trialSet.g_currentTr() << ' ' << interpolation.interpolate() << ' ';
	if (interpolation.interpolate() && 
		trialSet.g_subject() == interpolation.sub() && 
		trialSet.g_currentTr() > interpolation.nTrial() &&
		not interpInfo.eof() )
	{
		while (true)
		{
			cout << line;
			cout << "skipping lines\n";
			string line1, line2;
			getline(interpInfo, line1);
			getline(interpInfo, line2);
			interpolation.setSub("none"); // why initialization to none?
			if (interpInfo.good())
				interpolation = interpolation.extractInterpData(line1, line2);
			if (interpolation.nTrial() >= trialSet.g_currentTr())
				break;
			
		}
	}// if (interpolation.interpolate() && ...
	
	return interpolation;				
}