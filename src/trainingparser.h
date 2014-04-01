#pragma once

#include "types.h"
#include <fstream>
using std::fstream;

struct TrainingData {
    vector<double> input;
    vector<double> expectedOutput;
};

/*
 * Training Parser
 * 
 * Parser for trainingdata.
 */

class TrainingParser {
	public:
		TrainingParser(string filename);
		~TrainingParser();

		vector<TrainingData> Parse();
	private:
		bool OpenFileHandle(fstream &file, bool write);
		
		string _filename;
		int inputcount, outputcount;
};