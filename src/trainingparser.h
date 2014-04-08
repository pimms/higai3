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
		TrainingParser(string filename, const Topology *top);
		~TrainingParser();

		TrainingData ParseText();
	private:
		bool OpenFileHandle(fstream &file, bool write);
		
		string _filename;
		const Topology *_top;
};