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
	TrainingParser();
	~TrainingParser();

	TrainingSet ParseText(string filename, const Topology *top) const;
	TrainingSet ParseImages(CmdConfig *config) const;

private:
	bool OpenFileHandle(string filename, fstream &file, bool write) const;
	bool ReadLine(fstream &file, const Topology *top,
				  vector<double> &i, vector<double> &o) const;
};
