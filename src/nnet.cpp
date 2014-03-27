#include "nnet.h"
#include "layer.h"


NeuralNetwork::NeuralNetwork(Topology topology)
{
	if (topology.size() < 2) 
		throw runtime_error("At least 2 layers are required");
	
	Layer *prevLayer = NULL;

	for (int i=0; i<topology.size(); i++) {
		int real = topology[i].first;
		int bias = topology[i].second;
		Layer *layer = new Layer(prevLayer, real, bias);

		if (prevLayer) {
			prevLayer->SetNextLayer(layer);
		}

		prevLayer = layer;
		 
		_layers.push_back(layer);
	}
}

NeuralNetwork::~NeuralNetwork()
{
	for (int i=0; i<_layers.size(); i++)
		delete _layers[i];
}


void NeuralNetwork::SetLayerWeights(int layer, 
									vector< vector<double> > &weights)
{
	if (layer <= 0 || layer >= _layers.size())
		throw runtime_error("SetLayerWeights: invalid layer index");

	_layers[layer]->SetWeights(weights);
}


vector<double> NeuralNetwork::Calculate(const vector<double> &input)
{
	vector<double> output;
	int size = _layers.size();

	_layers[0]->SetInput(input);
	
	for (int i=1; i<size; i++) {
		_layers[i-1]->GetOutput(output);
		_layers[i]->SetInput(output);
	}

	_layers[size-1]->GetOutput(output);
	return output;
}
