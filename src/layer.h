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

	/* Override any existing weight values in the layer. The weights
	 * supplied will set the weights to the PREVIOUS layer. I.e., this
	 * method is valid on all layers which are not the input layer.
	 *
	 * The expected input is a M by N vector, where M is the number
	 * of REAL nodes in THIS layer, and N is the TOTAL number 
	 * of nodes in the previous layer.
	 */
	void SetWeights(vector< vector<double> > weights);

	~Layer();

	/* Returns the total number of nodes, including bias nodes.
	 */
	int GetNodeCount() const;

	/* Returns the number of REAL nodes
	 */
	int GetRealNodeCount() const;
	
	void GetOutput(vector<double>& output) const;
	void SetInput(const vector<double> &input);
	
private:
	Layer *_prevLayer;
	Layer *_nextLayer;
	vector<Neuron*> _neurons;
	vector<BiasNeuron*> _biases;
};
