#pragma once

#include "types.h"
#include <fstream>
using std::fstream;

struct TrainingData {
    vector<double> input;
    vector<double> expectedOutput;
};

typedef vector<TrainingData> TrainingSet;


/*
 * Training Parser
 * 
 * Parser for trainingdata.
 */
class TrainingParser {
public:
	TrainingParser(string filename, const Topology *top);
	~TrainingParser();

	TrainingSet ParseText();
private:
	bool OpenFileHandle(fstream &file, bool write) const;
	bool ReadLine(fstream &file, const Topology *top,
				  vector<double> &i, vector<double> &o) const;
	
	string _filename;
	const Topology *_top;
};
