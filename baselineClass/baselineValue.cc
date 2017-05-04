#include "main.ih"

double baselineValue(string& line)
{
	istringstream linedata(line);
	string Subject;
	size_t Trial;
	double psize;
	
	linedata >> Subject >> Trial >> psize;
	
	Base::setSubject(Subject);
	Base::setTrialIN(Trial);
	Base::setPsize(Target);
	
	
}
