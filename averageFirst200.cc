#include "main.ih"

size_t averageFirst200(string filename, size_t interval4baseline, string& lock2)
{
	string string2search = "TRIAL=1 ";
	string2search.append(lock2);
  ifstream subInfoFile("subnames.txt");
  if (!subInfoFile.is_open())
    cout << "Unable to open subnames.txt\n";
  else
  { 
    ofstream outputfile;
//     filename.append("_averageFirst200.asc");
		string otherBitFilename = "averageFirst" + to_string(interval4baseline) + ".asc";
		filename.append(otherBitFilename);
		
    outputfile.open(filename);
// 		outputfile << "pp" << '\t'  << "tr" << '\t'  << 'x' << '\t' << 'y' << '\t' << "psize"  << '\n';  
    
    outputfile << "pp" << '\t'  << "tr" << '\t' << "psize"  << '\n';  
    string subID;
		while(getline(subInfoFile, subID))
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
						
					if (line.find(string2search) != string::npos)
					{
// 						exportBaseline(xpos, ypos, psize, subID, "1", outputfile);
						exportBaseline(psize, subID, "1", outputfile);
						eyetrackingFile.close();
						cout << " processed \n";
						
						vector<double>().swap(xpos);
						vector<double>().swap(ypos);
						vector<double>().swap(psize);
						
						break;
					}
						
					eye.extractData(line);
					
// 					if (eye.isMSG() == false)
					if (eye.isValid() == true)
					{
			// 	    size_t lines2include = 100; // 200 ms is 50 lines each sampled every 4 seconds
						size_t lines2include = interval4baseline / 4; // 200 ms is 50 lines each sampled every 4 seconds
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
					}
				} // end of "while(getline(eyetrackingFile, line))"
      } // end "if(! eyetrackingFile.is_open()") 
    } // end "while(getline(subInfoFile, subID))"
    

    subInfoFile.close();
    outputfile.close();
  }
  return 1;
}