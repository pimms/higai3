#pragma once

#include "types.h"
#include <fstream>
using std::fstream;


class NeuralNetwork;


/* Network Parser
 *
 * Class for parsing a Neural Network from file and creating
 * the correct neural network. The format can be found in 
 * /FORMAT_SPEC.
 */
class NetworkParser {
public:
	NetworkParser(string file);

	NeuralNetwork* Parse();
	void Save(const NeuralNetwork*);

private:
	/* The returned handle is read-only if "write" is false, and
	 * read-write if it is true.
	 */
	bool OpenFileHandle(fstream &file, bool write);

	Topology ReadTopology(fstream &file, int layers);
	vector< vector<double> > ReadWeights(fstream &file, 
										 int prevCount, int curCount);
		
	string _filename;
};
