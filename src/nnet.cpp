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


vector<double> NeuralNetwork::Calculate(const vector<double> &input)
{
	vector<double> finalOutput;

	for (int i=0; i<_layers.size()-1; i++) {
		vector<double> output;
		
		_layers[i]->GetOutput(output);
		_layers[i+1]->SetInput(output);
	}

	_layers[_layers.size()-1]->GetOutput(finalOutput);
	return finalOutput;
}
