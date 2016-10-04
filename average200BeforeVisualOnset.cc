#include "main.ih"

size_t average200BeforeVisualOnset(string filename, size_t interval4baseline, string& lock2)
{
	ifstream subInfoFile("subnames.txt");
  if (!subInfoFile.is_open())
    cout << "Unable to open subnames.txt\n";
  else
  { 
    ofstream outputfile;
		string otherBitFilename = "average" + to_string(interval4baseline) + "_BeforeVisualOnset.asc";
		filename.append(otherBitFilename);
		outputfile.open(filename);
// 		outputfile << "pp" << '\t'  << "tr" << '\t'  << "x" << '\t' << "y" << '\t' << "psize"  << '\n';  
		outputfile << "pp" << '\t'  << "tr" << '\t'  << "psize"  << '\n';  
		
    string subID;
    while (getline(subInfoFile, subID))
    {
      ifstream eyetrackingFile(subID);
      if (! eyetrackingFile.is_open())
      {
				cout << "Unable to open eyetracking datafile " << subID << '\n';
				break;
      }
      else
      {
				Dataline eye;
				
				vector<double> xpos;
				vector<double> ypos;
				vector<double> psize;
				
				cout << subID << ' ';
				string line;
				while (getline(eyetrackingFile, line))
				{
					/* ignore lines which are fix or sac messages */
					while ((line.find("FIX") != string::npos) || (line.find("SAC") != string::npos) || 
						(line.find("BLINK") != string::npos) || (line.find("BUTTON") != string::npos))
						getline(eyetrackingFile, line);
						
					if (line.find(lock2) != string::npos)
					{
						string trialNum = line.substr(line.find("=") + 1, 
																					line.find("onset") - (line.find("=") + 1));
						
						if (xpos.size() > 10)
							exportBaseline(psize, subID, trialNum, outputfile);
// 						exportBaseline(xpos, ypos, psize, subID, trialNum, outputfile);
						
						vector<double>().swap(xpos);
						vector<double>().swap(ypos);
						vector<double>().swap(psize);
						
						while (getline(eyetrackingFile, line))
						{
							if (line.find("TRIAL ENDS") != string::npos)
								break;
						}
					}// if (line.find("onsetVisualStim") != string::npos)
						
					eye.extractData(line);
					
					if (eye.isValid() == true)
					{
						size_t lines2include = interval4baseline / 4;// 50; // 200 ms is 50 lines each sampled every 4 seconds
						if (xpos.size() >= lines2include)
						{
							xpos.push_back(eye.g_xpos());
							xpos.erase(xpos.begin());
							ypos.push_back(eye.g_ypos());
							ypos.erase(ypos.begin());
							psize.push_back(eye.g_psize());
							psize.erase(psize.begin());
						}
						else
						{
							xpos.push_back(eye.g_xpos());
							ypos.push_back(eye.g_ypos());
							psize.push_back(eye.g_psize());
						}
					} // if (eye.isMSG() == false)
				} // end of "while(getline(eyetrackingFile, line))"
				
				eyetrackingFile.close();
				cout << " processed \n";
      } // end "if(! eyetrackingFile.is_open()") 
    } // end "while(getline(subInfoFile, subID))"
    
    subInfoFile.close();
    outputfile.close();
  }
  return 1;
}