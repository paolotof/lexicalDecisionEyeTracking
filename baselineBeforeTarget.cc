#include "main.ih"

size_t baselineBeforeTarget(string filename, string& eye2read, size_t interval4baseline) {
	cout << eye2read << " \n";
	cout << "Computing baseline before AUDIO target onset\n";
	ifstream eyetrackingFile(eye2read);
  if (!eyetrackingFile.is_open())
		cout << "Unable to open " << eye2read << " \n";
  else { 
    ofstream outputfile;
		string otherBitFilename = "averageBeforeTarget" + 
			to_string(interval4baseline) + ".asc";
		filename.append(otherBitFilename);
    outputfile.open(filename);
		outputfile << "pp" << '\t'  << "tr" << '\t' << "psize"  << '\n';  
    
    string eyeData;
		getline(eyetrackingFile, eyeData); // read header separately to not
		// include in the vector which will be averaged 
		// now read data
		vector<double> psize;
		while(getline(eyetrackingFile, eyeData)){
			double clockTime, time, pupilSize, fix;
			size_t pp, trial; 
			istringstream linedata(eyeData);
			linedata >> pp >> clockTime >> time
				>> trial >> pupilSize >> fix;
			if ((time > 0.0) && (time <= 4.0)){
				outputfile << pp << '\t' << trial << '\t' << 
					vectorMean(psize) << '\n';
				vector<double>().swap(psize); }
			psize.push_back(pupilSize);
			size_t nSamplesPerLine = 4;
			size_t lines2include = interval4baseline / nSamplesPerLine;
			if (psize.size() >= lines2include)
				psize.erase(psize.begin()); } // END: "while(getline(eyetrackingFile, eyeData))"
    eyetrackingFile.close();
    outputfile.close(); }
  return 1;
}
