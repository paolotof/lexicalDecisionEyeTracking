#include "main.ih"

void exportInterpolation(string& nameInterpolationFile, double blinkDuration, 
		TrialInfo trialSet, 
		double tVect, double xVect, double yVect, double pVect, 
		double medianT, double medianX, double medianY, double medianP){
	
	std::ofstream shortBlinksReport;
	shortBlinksReport.open(nameInterpolationFile, std::ios_base::app);
// 	cout << "exporting function, does it work?\n";
// 	cout << medianT << ' ' << medianX << ' ' << medianY << ' ' << medianP << '\n';
	shortBlinksReport << trialSet.g_subject() << ' ' << trialSet.g_currentTr() << ' '; 
	size_t old_precision = shortBlinksReport.precision();
	shortBlinksReport.precision(10);
	shortBlinksReport << medianT;
	shortBlinksReport.precision(old_precision);
	shortBlinksReport << ' ' << medianX << ' ' << medianY << ' ' << medianP << '\n';
	// end interpolation
	shortBlinksReport << trialSet.g_subject() << ' ' << trialSet.g_currentTr() << ' ';
	shortBlinksReport.precision(10);
	shortBlinksReport << tVect; 
	shortBlinksReport.precision(old_precision);
	shortBlinksReport << ' ' << xVect << ' ' << yVect  << ' ' << pVect  
	<< ' ' << blinkDuration //<< ' ' << &type << '\n'; // n. lines to interpolate, NOT, those are computed on the basis of the length of the interval.
// 	<< ' ' << blinkDuration \ 4 
	<< '\n'; // n. lines to interpolate, NOT, those are computed on the basis 
	shortBlinksReport.close();
}
