#include "main.ih"

vector<int> findOnsets(string& file2read)
{
	ifstream iFile(file2read);
	vector<int> onsetTimes;
	if (iFile.is_open())
	{
		string line;
		while ( getline (iFile,line) )
		{
			if (line.find("onsetSoundStim") != string::npos)
			{
				istringstream linedata(line);
				string msg, trialNum, objOnset;
				int TargetStarts;
				linedata >> msg >> TargetStarts >> trialNum >> objOnset; 
				
// 				cout << line << '\n';
// 				cout << TargetStarts << '\n';
				onsetTimes.push_back(TargetStarts);
			}
		}
		iFile.close();
	}
	else cout << "Unable to open file";

// 	cout << onsetTimes.size() << '\n';
	return onsetTimes; 
}