#include "neuron.h"
#include "layer.h"


Neuron::Neuron(Layer *previousLayer)
{
	if (previousLayer) {
		_input.resize(previousLayer->GetNodeCount());
		_weights.resize(previousLayer->GetNodeCount(), 1.0);
	} else {
		// The node is an input-neuron or a bias. It can only take 
		// one input, and it's weight is always 1.0.
		_input.resize(1);
		_weights.push_back(1.0);
	}
}

Neuron::~Neuron()
{

}


void Neuron::SetInput(int nodeIndex, double value)
{
	if (nodeIndex < 0 || nodeIndex >= _input.size()) {
		throw runtime_error("Invalid node index");
	}

	_input[nodeIndex] = value;
}

double Neuron::GetOutput() const
{
	double netInput = 0.0;

	for (int i=0; i<_input.size(); i++) {
		netInput += _input[i] * _weights[i];
	}

	return Activation(netInput);
}

void Neuron::SetWeights(const vector<double> &weights)
{
	if (weights.size() != _weights.size()) 
		throw runtime_error("Neuron::SetWeights() - invalid input size");

	_weights = weights;
}

vector<double> Neuron::GetWeights() const
{
	return _weights;
}


/* Private Methods */

double Neuron::Activation(double netInput) const
{
	return netInput;
}


/* BiasNeuron */

BiasNeuron::BiasNeuron()
	: 	Neuron(NULL)
{

}

double BiasNeuron::GetOutput() const
{
	return 1.0;
}
