#include "main.ih"

int main(int argc, char **argv)
try
{
	if (argc != 2)
		cout << "input requires two arguments as in: ./test example.txt";

	string filename = argv[1];
	vector<int> onsetTimes;
	onsetTimes = findOnsets(filename);

	cout << "onset extracted\n";
	
	string outFile = filename;
	
// 	cout << filename <<'\n';
// 	cout << (filename.find("/") == string::npos) <<'\n';
	if (filename.find("/") == string::npos)
		outFile.insert(0, "mod_");
	else 
	{ // not necessary if running from same folder
		string str2search = "/eyetracker/";
		size_t pos2insert = filename.find(str2search);
		pos2insert = pos2insert + str2search.length();
		outFile.insert(pos2insert, "mod_");
	}
	
	cout << outFile << '\n';

 	writeFile(filename, onsetTimes, outFile);
	cout << "onsets added 2 file\n";
	
	return 0;	
}
catch (...)
{
    return 1;
}
