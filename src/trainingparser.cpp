#include "trainingparser.h"
#include "nnet.h"
#include "util.h"
#include <cstdlib>
#include <iostream>

TrainingParser::TrainingParser(string filename) 
		: _filename(filename) 
{
}

TrainingParser::~TrainingParser()
{
}

vector<TrainingData> TrainingParser::Parse() 
{

	vector<TrainingData> returndata;

	fstream filename;
	if (!OpenFileHandle(filename, false))
	{
		return returndata;
	}

	int trainingcount;
	filename >> trainingcount;
	
	TrainingData data;

	for (int i = 0; i < trainingcount; ++i)
	{
		printf("Trainingcount: %d\n", i);
		filename >> inputcount;
		filename >> outputcount;
		double nr;

		printf("Input:\n");
		for (int i = 0; i < inputcount; ++i)
		{
			filename >> nr;
			data.input.push_back(nr);
			printf("%f\n", nr);
		}

		printf("Output:\n");
		for (int i = 0; i < outputcount; ++i)
		{
			filename >> nr;
			data.expectedOutput.push_back(nr);
			printf("%f\n", nr);
		}

		returndata.push_back(data);
	}

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