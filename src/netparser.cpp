#include "netparser.h"
#include "nnet.h"
#include "util.h"
#include <cstdlib>


NetworkParser::NetworkParser(string filename)
	:	_filename(filename)
{
}


NeuralNetwork* NetworkParser::Parse()
{
	fstream file;
	if (!OpenFileHandle(file, false)) 
		return NULL;
	
	int layerCount = 0;
	NeuralNetwork *network = NULL;
	Topology topology;
	vector<LayerWeights> weights;
	string weightType;

	// Read the topology
	file >> layerCount;
	topology = ReadTopology(file, layerCount);

	// Read the type of weights (random or defined)
	file >> weightType;
	if (weightType == "RAND") {
		RandomizeAllWeights(weights, topology, file);
	} else if (weightType == "PREDEF") {
		ReadAllWeights(weights, topology, file);
	} else {
		throw runtime_error("[NetworkParser::Parse()]"
							" Invalid weight type: " + weightType);
		return NULL;
	}


	// Create the network and assign the weights
	network = new NeuralNetwork(topology);
	for (int i=0; i<weights.size(); i++) {
		network->SetLayerWeights(i+1, weights[i]);
	}
	
	return network;
}

void NetworkParser::Save(const NeuralNetwork *network)
{
	throw runtime_error("NOT IMPLEMENTED: NetworkParser::Save()");
}


/* Private Method */

bool NetworkParser::OpenFileHandle(fstream &file, bool write)
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


Topology NetworkParser::ReadTopology(fstream &file, int layers)
{
	Topology topology;

	for (int i=0; i<layers; i++) {
		pair<int,int> layer;
		
		file >> layer.first;
		file >> layer.second;

		topology.push_back(layer);
	}

	return topology;
}

void NetworkParser::ReadAllWeights(vector<LayerWeights> &weights,
								   Topology &topology,
								   fstream &file)
{
	weights.clear();

	// Read the weights 
	for (int i=1; i<topology.size(); i++) {
		int prevCount = topology[i-1].first + topology[i-1].second;
		int curCount  = topology[i].first + topology[i].second;

		LayerWeights layerWeight;
		layerWeight = ReadLayerWeights(file, prevCount, curCount);
		weights.push_back(layerWeight);
	}
}

void NetworkParser::RandomizeAllWeights(vector<LayerWeights> &weights,
										Topology &topology,
										fstream &file)
{
	weights.clear();

	double min = 1.0;
	double max = 0.0;
	file >> min >> max;

	if (min > max) 
		throw runtime_error("RandomizeWeights: invalid values");

	for (int i=1; i<topology.size(); i++) {
		int prevCount = topology[i-1].first + topology[i-1].second;
		int curCount  = topology[i].first + topology[i].second;

		LayerWeights layerWeight;
		layerWeight = RandomizeLayerWeights(min, max, prevCount, curCount);
		weights.push_back(layerWeight);
	}
}

LayerWeights NetworkParser::ReadLayerWeights(fstream &file, 
										int prevCount, int curCount)
{
	LayerWeights weights;
	weights.resize(curCount);

	for (int i=0; i<curCount; i++) {
		for (int j=0; j<prevCount; j++) {
			double d;
			file >> d;
			weights[i].push_back(d);
		}
	}

	return weights;
}

LayerWeights NetworkParser::RandomizeLayerWeights(double min, double max,
												  int prev, int cur)
{
	LayerWeights weights;
	weights.resize(cur);	

	for (int i=0; i<cur; i++) {
		for (int j=0; j<prev; j++) {
			double d = RandRange(min, max);
			weights[i].push_back(d);
		}
	}

	return weights;
}
