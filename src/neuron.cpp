#include "neuron.h"
#include "layer.h"

#include <math.h>


Neuron::Neuron(Layer *previousLayer)
	:	_gain(1.0),
		_wgain(1.0)
{
	if (previousLayer) {
		int prevCount = previousLayer->GetNodeCount();
		_input.resize(prevCount);
		_weights.resize(prevCount, 1.0);
		_delta.resize(prevCount);
	} else {
		// The node is an input-neuron or a bias. It can only take 
		// one input, and it's weight is always 1.0.
		_input.resize(1);
		_weights.push_back(1.0);
		_delta.resize(1);
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

double Neuron::GetInput(int nodeIndex) const
{
	if (nodeIndex < 0 || nodeIndex >= _input.size()) {
		throw runtime_error("Invalid node index");
	}

	return _input[nodeIndex];
}


double Neuron::GetOutput() const
{
	double netInput = 0.0;

	for (int i=0; i<_input.size(); i++) {
		netInput += _input[i] * _weights[i];
	}

	return Activation(netInput);
}


void Neuron::UpdateWeight(int weightIndex, double delta)
{
	if (weightIndex < 0 || weightIndex >= _weights.size()) {
		throw runtime_error("[UpdateWeight()]: Invalid weight index");
	}

	_weights[weightIndex] += delta;
	_delta[weightIndex] = delta;
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


void Neuron::SetWeightGain(double wgain)
{
	_wgain = wgain;
}

double Neuron::GetGain() const
{
	return _gain;
}

double Neuron::GetWeightGain() const
{
	return _wgain;
}

double Neuron::GetDelta(int index) const
{
	if (index < 0 || index >= _delta.size()) {
		throw runtime_error("[GetDelta()]: Invalid index");
	}

	return _delta[index];
}


/* Private Methods */

double Neuron::Activation(double netInput) const
{
	return 1.0 / (1.0 + exp(-netInput));
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
