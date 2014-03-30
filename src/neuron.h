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
	double GetInput(int nodeIndex) const;

	virtual double GetOutput() const;

	void UpdateWeight(int weightIndex, double delta);
	void SetWeights(const vector<double> &weights);

	vector<double> GetWeights() const;

	double GetGain() const;
	double GetWeightGain() const;
	double GetDelta(int index) const;

private:
	double Activation(double netInput) const;

	vector<double> _input;

	// The weights of the connections to the PREVIOUS layer.
	vector<double> _weights;

	double _gain;
	double _wgain;
	vector<double> _delta;
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
