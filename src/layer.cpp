#include "layer.h"
#include "neuron.h"


Layer::Layer(Layer *prevLayer, int realNodes, int biasNodes)
	: 	_nextLayer(NULL)
{
	if (realNodes <= 0) 
		throw runtime_error("A layer requires at least 1 real node");
	if (biasNodes < 0) 
		throw runtime_error("Cannot create negative # of bias nodes");

	for (int i=0; i<realNodes; i++) {
		Neuron *neuron = new Neuron(prevLayer);
		_neurons.push_back(neuron);
	}

	for (int i=0; i<biasNodes; i++) {
		BiasNeuron *bias = new BiasNeuron();
		_biases.push_back(bias);
	}
}

void Layer::SetNextLayer(Layer *next)
{
	_nextLayer = next;
}


Layer::~Layer() 
{
	for (int i=0; i<_neurons.size(); i++) 
		delete _neurons[i];
	for (int i=0; i<_biases.size(); i++) 
		delete _biases[i];
}


int Layer::GetNodeCount() const
{
	return _neurons.size() + _biases.size();
}


void Layer::GetOutput(vector<double>& output) const
{
	for (int i=0; i<_neurons.size(); i++) 
		output.push_back(_neurons[i]->GetOutput());
	for (int i=0; i<_biases.size(); i++) 
		output.push_back(_biases[i]->GetOutput());
}

void Layer::SetInput(const vector<double>& input)
{
	for (int i=0; i<_neurons.size(); i++) {
		for (int j=0; j<input.size(); j++) {
			_neurons[i]->SetInput(j, input[j]);
		}
	}
}

