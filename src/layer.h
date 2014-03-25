#pragma once

#include "types.h"

class Neuron;
class BiasNeuron;


class Layer {
public:
	/* Create a layer with "realnodes" real nodes and "biasnodes"
	 * bias nodes. The real nodes take an input from the layer
	 * before them. The bias nodes do not.
	 */
	Layer(Layer *prevLayer, int realnodes, int biasnodes);
	void SetNextLayer(Layer *next);

	~Layer();

	/* Returns the total number of nodes, including bias nodes.
	 */
	int GetNodeCount() const;
	
	void GetOutput(vector<double>& output) const;
	void SetInput(const vector<double> &input);
	
private:
	Layer *_nextLayer;
	vector<Neuron*> _neurons;
	vector<BiasNeuron*> _biases;
};
