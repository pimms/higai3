#include "trainingparser.h"
#include "nnet.h"
#include "util.h"
#include <cstdlib>
#include <iostream>

TrainingParser::TrainingParser(string filename, const Topology *top) 
		: _filename(filename), _top(top)
{
}

TrainingParser::~TrainingParser()
{
}

TrainingSet TrainingParser::ParseText() 
{
	TrainingSet returndata;

	fstream file;
	if (!OpenFileHandle(file, false)) {
		return returndata;
	}

	vector<double> in, target;	
	while (ReadLine(file, _top, in, target)) {
		TrainingData td;
		td.input = in;
		td.expectedOutput = target;

		returndata.push_back(td);
	}
	
	return returndata;
}


/* Private Methods */
bool TrainingParser::OpenFileHandle(fstream &file, bool write) const
{
	std::ios::openmode mode = std::ios::in;

	if (write) {
		mode |= std::ios::out;
	}

	file.open(_filename, mode);
	if (!file.is_open()) {
		throw runtime_error("Failed to open file: " + _filename);
		return false;
	}

	return true;
}

bool TrainingParser::ReadLine(fstream &file, const Topology *top,
							  vector<double> &in, 
							  vector<double> &output) const
{
	int inCount = (*_top)[0].first;
	int outCount = (*_top)[_top->size()-1].first;
	in.clear();
	output.clear();
	
	vector<double> read;
	for (int i=0; i<inCount + outCount; i++) {
		double d;
		if (!(file >> d)) break;
		read.push_back(d);
	}

	if (read.size() == inCount + outCount) {
		for (int i=0; i<inCount; i++) {
			in.push_back(read[i]);
		}
		
		for (int i=0; i<outCount; i++) {
			output.push_back(read[inCount+i]);
		}

		return true;
	}

	return false;
}
