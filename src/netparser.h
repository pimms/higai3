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

	/* Read all weights according to the topology into "weights".
	 */
	void ReadAllWeights(vector<LayerWeights> &weights, 
						Topology &topology, 
						fstream &file);

	/* Randomize the weights according to the topology
	 */
	void RandomizeAllWeights(vector<LayerWeights> &weights,
							 Topology &topology,
							 fstream &file);

	/* Read the weights of a single layer
	 */
	LayerWeights ReadLayerWeights(fstream &file, int prev, int cur);
	
	/* Randomize the weights of a single layer
	 */
	LayerWeights RandomizeLayerWeights(double min, double max, 
									   int prevCount, int curCount);
	
		
	string _filename;
};
