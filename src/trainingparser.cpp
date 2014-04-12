#include "trainingparser.h"
#include "imageparser.h"
#include "nnet.h"
#include "util.h"
#include <cstdlib>
#include <iostream>

#include <sstream>
using std::stringstream;


TrainingParser::TrainingParser() 
{
}

TrainingParser::~TrainingParser()
{
}

TrainingSet TrainingParser::ParseText(string filename, 
									  const Topology *top) const
{
	TrainingSet returndata;

	fstream file;
	if (!OpenFileHandle(filename, file, false)) {
		return returndata;
	}

	vector<double> in, target;	
	while (ReadLine(file, top, in, target)) {
		TrainingData td;
		td.input = in;
		td.expectedOutput = target;
		td.identification = "Anonymous training data";

		returndata.data.push_back(td);
	}
	
	return returndata;
}

TrainingSet TrainingParser::ParseImages(CmdConfig *config) const
{
	TrainingSet tset;
	ImageParser imgParser;

	tset.samples = config->samplesPerCharacter;
		
	for (int i=0; i<26; i++) {
		for (int j=0; j<20; j++) {
			char letter = 'A' + i;
			stringstream ss;
			ss << "img/" << letter << j+1 << ".jpg";
			string file = ss.str();
			
			TrainingData tdata;
			Image img;
			
			img = imgParser.ParseImage(file.c_str(), config->scalefactor);
			
			for (int k=0; k<img.width*img.height; k++) {
				unsigned char pixel = img.pixels[k];
				double val = (double)pixel / 255.0;
				tdata.input.push_back(val);
			}

			tdata.expectedOutput.resize(26, 0.0);
			tdata.expectedOutput[i] = 1.0;
			tdata.identification = file;
			tset.data.push_back(tdata);
		}
	}

	return tset;
}


/* Private Methods */
bool TrainingParser::OpenFileHandle(string filename, fstream &file, 
									bool write) const
{
	std::ios::openmode mode = std::ios::in;

	if (write) {
		mode |= std::ios::out;
	}

	file.open(filename, mode);
	if (!file.is_open()) {
		throw runtime_error("Failed to open file: " + filename);
		return false;
	}

	return true;
}

bool TrainingParser::ReadLine(fstream &file, const Topology *top,
							  vector<double> &in, 
							  vector<double> &output) const
{
	int inCount = (*top)[0].first;
	int outCount = (*top)[top->size()-1].first;
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
