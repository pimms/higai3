#pragma once

#include "types.h"
#include <fstream>
using std::fstream;


/*
 * Training Parser
 * 
 * Parser for trainingdata.
 */

class TrainingParser {
	public:
		TrainingParser(string filename);
		~TrainingParser();
	private:
		string _filename;
};