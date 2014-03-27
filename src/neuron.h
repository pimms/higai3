#pragma once

#include "types.h"

class Layer;


/* Neuron
 *
 * The lowest level node in a neural network.
 */
class Neuron {
public:
	Neuron(Layer *previousLayer);
	virtual ~Neuron();

	void SetInput(int nodeIndex, double value);

	virtual double GetOutput() const;

	void SetWeights(const vector<double> &weights);

private:
	double Activation(double netInput) const;

	vector<double> _input;

	// The weights of the connections to the PREVIOUS layer.
	vector<double> _weights;
};


/* BiasNeuron
 *
 * Subclass of Neuron. Overrides the GetOutput() method, 
 * which always returns 1.0 in a bias. 
 */
class BiasNeuron : public Neuron {
public:
	BiasNeuron();
	virtual double GetOutput() const;	
};
