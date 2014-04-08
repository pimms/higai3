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

TrainingData TrainingParser::ParseText() 
{

	int inCount = (*_top)[0].first;
	int outCount = (*_top)[_top->size()-1].first;
	TrainingData returndata;

	fstream filename;
	if (!OpenFileHandle(filename, false))
	{
		return returndata;
	}

	int trainingcount;
	filename >> trainingcount;
	
	TrainingData data;

	return returndata;
}

bool TrainingParser::OpenFileHandle(fstream &file, bool write)
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