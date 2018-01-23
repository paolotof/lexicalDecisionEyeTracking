#include "main.ih"

void writeFile(string& file2read, vector<int> onsetTimes, string& filename)
{
	ifstream iFile(file2read);
	size_t trNum = 0;
	if (not iFile.is_open())
		cout << "Unable to open file";
	else 
	{
		ofstream oFile (filename);
		string line;
		while ( getline (iFile,line) )
		{
			if (line.find("TRIAL STARTS") != string::npos)
			{
				trNum++;
				string bit2insert = "=";
				bit2insert.append(to_string(trNum));
				line.insert(line.find(" STARTS"), bit2insert);
				line = line + ' ';
				line.append(to_string(onsetTimes[trNum-1]));
// 				cout << line << '\n'; 
			}
			oFile << line << '\n';
		}
		iFile.close();
		oFile.close();
	}
}