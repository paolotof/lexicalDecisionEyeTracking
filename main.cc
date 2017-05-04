#include "main.ih"

int main(int argc, char **argv)
try
{
	string dir2save = "";
	string outputfile = dir2save + "tbt_";
	
	ifstream trialinfofile("trials2beIncluded.txt");
	string lock2 = "STARTS";

	checkBlinksAndGetInterpBounds("trials2beIncluded.txt", outputfile, lock2);

	outputfile.append("_");
	size_t timeBefore = 1000;
	timeBefore = 0; // now we are starting at the onset of the word
	outputfile += std::to_string(timeBefore);
	processData("trials2beIncluded_noBlinks.txt", outputfile, timeBefore, lock2);
	
	extractSaccades("trials2beIncluded_noBlinks.txt", outputfile, lock2);
	string tmp = outputfile;
	tmp.append("_ALL_medianInterp.asc");
	binData(tmp, 5); //lines2bin = 5
	
	lock2 = "onsetSoundStim";
	cout << "Baseline Before " << lock2 << " 1st trial ONLY\n\n";
 	size_t baselineInt = 200;
	averageFirst200(outputfile, baselineInt, lock2);
	
	cout << "Baseline Before " << lock2 << " Every Trial\n\n";
	average200BeforeVisualOnset(outputfile, baselineInt, lock2);

// // 	baselineOne();
// // 	baselineTwo();
	string outputfileName = "tbt_eyeData_Baseline1and2.asc";
	baselineOneAndTwo(outputfileName);
	binData(outputfileName, 5); //lines2bin = 5
}
catch (...)
{
    return 1;
}
