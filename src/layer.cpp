#include "layer.h"
#include "neuron.h"


Layer::Layer(Layer *prevLayer, int realNodes, int biasNodes)
	: 	_prevLayer(prevLayer),
		_nextLayer(NULL)
{
	if (realNodes <= 0) 
		throw runtime_error("A layer requires at least 1 real node");
	if (biasNodes < 0) 
		throw runtime_error("Cannot create negative # of bias nodes");

	for (int i=0; i<realNodes; i++) {
		Neuron *neuron = new Neuron(_prevLayer);
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


void Layer::SetWeights(vector< vector<double> > weight) 
{
	// Assert valid input
	if (weight.size() != _neurons.size())
		throw runtime_error("[SetWeights] Invalid number of input nodes");
	for (int i=0; i<weight.size(); i++)
		if (weight[i].size() != _prevLayer->GetNodeCount())
			throw runtime_error("[SetWeights] Invalid number of weights");

	for (int i=0; i<weight.size(); i++) {
		_neurons[i]->SetWeights(weight[i]);
	}
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

int Layer::GetRealNodeCount() const
{
	return _neurons.size();
}


void Layer::GetOutput(vector<double>& output) const
{
	output.clear();

	for (int i=0; i<_neurons.size(); i++) 
		output.push_back(_neurons[i]->GetOutput());
	for (int i=0; i<_biases.size(); i++) 
		output.push_back(_biases[i]->GetOutput());
}

void Layer::SetInput(const vector<double>& input)
{
	if (_prevLayer) {
		// Set the input for ALL nodes
		for (int i=0; i<_neurons.size(); i++) {
			for (int j=0; j<input.size(); j++) {
				_neurons[i]->SetInput(j, input[j]);
			}
		}
	} else {
		// Map each input to each node
		for (int i=0; i<_neurons.size(); i++) {
			_neurons[i]->SetInput(0, input[i]);
		}
	}
}


void Layer::PrintInformation() const
{
	LayerWeights lw;
	int wcount = 0;

	printf("+++++|");

	for (int i=0; i<_neurons.size(); i++) {
		printf(" Neuron %4i |", i);
		lw.push_back(_neurons[i]->GetWeights());
	}

	wcount = lw[0].size();
	printf("\n");

	for (int i=0; i<wcount; i++) {
		printf("W%3d |", i);

		for (int j=0; j<lw.size(); j++) {
			printf(" %11g |", lw[j][i]);
		}
		
		printf("\n");
	}
}
