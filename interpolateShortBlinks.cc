#include "main.ih"

// size_t interpolateShortBlinks(ofstream& outputfile, TrialInfo trialSet, Dataline preBlink, 
// 															Dataline postBlink, size_t lines2interp, size_t timeBefore)
size_t interpolateShortBlinks(ofstream& outputfile, TrialInfo trialSet, Dataline preBlink, 
															Dataline postBlink, size_t lines2interp)
{
  
//   cout << "S";
  // interpolate
  double dataArray[4] = {0, 0, 0, 0};
  for (double iter = (lines2interp - 1); iter >= 0; iter--)
  {
    dataArray[0] = linearInterpolate(postBlink.g_time(), preBlink.g_time(), iter/lines2interp);
    dataArray[1] = linearInterpolate(postBlink.g_xpos(), preBlink.g_xpos(), iter/lines2interp);
    dataArray[2] = linearInterpolate(postBlink.g_ypos(), preBlink.g_ypos(), iter/lines2interp);
    dataArray[3] = linearInterpolate(postBlink.g_psize(), preBlink.g_psize(), iter/lines2interp);
    
//     size_t timeBefore = 400;
// 		if (dataArray[0] >= (trialSet.g_targetOnset() - timeBefore))
		if (dataArray[0] >= trialSet.g_startExport())
		{
			trialSet.addOneBin();
			Dataline eye;
			eye.setTime(dataArray[0]);
			eye.setSize(dataArray[3]);
			writeOut(eye, outputfile, trialSet);
      //AssignAreaAndWriteOut(dataArray, outputfile, trialSet, charlotte);
    }
      //trialSet  = AssignAreaAndWriteOut(dataArray, outputfile, trialSet, charlotte);
    
  } // end for loop "for (double iter = (lines2interp-1); iter > 0; --iter)"
  
  //return(trialSet);
  return lines2interp;
}