#include "netparser.h"
#include "nnet.h"


NetworkParser::NetworkParser(string filename)
	:	_filename(filename)
{

}


NeuralNetwork* NetworkParser::Parse()
{
	fstream file;
	if (!OpenFileHandle(file, false)) 
		return NULL;
	
	NeuralNetwork *network = NULL;
	int layerCount = 0;
	Topology topology;
	vector< vector< vector<double> > > weights;

	// Read the topology
	file >> layerCount;
	topology = ReadTopology(file, layerCount);

	// Read the weights 
	for (int i=1; i<topology.size(); i++) {
		int prevCount = topology[i-1].first + topology[i-1].second;
		int curCount  = topology[i].first + topology[i].second;

		vector< vector<double> > layerWeight;
		layerWeight = ReadWeights(file, prevCount, curCount);
		weights.push_back(layerWeight);
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

vector< vector<double> > NetworkParser::ReadWeights(fstream &file, 
												 	int prevCount,
													int curCount)
{
	vector< vector<double> > weights;
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
