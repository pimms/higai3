#pragma once

#include "types.h"
#include <fstream>
using std::fstream;
using std::ofstream;
using std::ifstream;

struct TrainingData {
	int trainingCount;
	string identification;
    vector<double> input;
    vector<double> expectedOutput;

	TrainingData()
		: trainingCount(0)
	{}
};

struct TrainingSet {
	vector<TrainingData> data;
	int samples;

	TrainingSet() : samples(20) {}
};


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
