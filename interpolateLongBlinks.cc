#include "main.ih"

// bool interpolateLongBlinks(ofstream& outputfile, TrialInfo trialSet, Interpdata interpolation, size_t timeBefore)
bool interpolateLongBlinks(ofstream& outputfile, TrialInfo trialSet, Interpdata interpolation)
{
  
//   cout << "L";
  bool updateInterp = false;
  // interpolate
  size_t lines2interp = (interpolation.iEnd() - interpolation.iBegin()) / 4;
  double dataArray[4] = {0, 0, 0, 0};
// 	for (double iter = lines2interp; iter > 0; --iter)
	for (double iter = lines2interp; iter >= 0; --iter)
  {
    dataArray[0] = linearInterpolate(interpolation.iEnd(), interpolation.iBegin(), 
				     iter/lines2interp);
    dataArray[1] = linearInterpolate(interpolation.endX(), interpolation.begX(), 
				     iter/lines2interp);
    dataArray[2] = linearInterpolate(interpolation.endY(), interpolation.begY(), 
				     iter/lines2interp);
    dataArray[3] = linearInterpolate(interpolation.endP(), interpolation.begP(), 
				     iter/lines2interp);
// 		if ((dataArray[0] >= (trialSet.g_targetOnset() - timeBefore)) && (dataArray[0] <= trialSet.timeIsUp()))
		if ((dataArray[0] >= trialSet.g_startExport()) && (dataArray[0] <= trialSet.timeIsUp()))
		{ // time of interest boundaries
			trialSet.addOneBin();
			// writeOut uses the eye.g_time() and eye.g_psie() of Dataline structure, so let's update those so that it works:
			Dataline eye;
			eye.setTime(dataArray[0]);
			eye.setSize(dataArray[3]);
			writeOut(eye, outputfile, trialSet);
			updateInterp = true;
		// because there are more types of blinks which get interpolated (e.g. short blinks)
    }
  } // end for loop "for (double iter = (lines2interp-1); iter > 0; --iter)"
// 	cout << updateInterp << ' ';
  return(updateInterp);
}