#pragma once

#include "types.h"

class Layer;


class NeuralNetwork {
public:
	NeuralNetwork(Topology topology);
	~NeuralNetwork();

	/* Sets the weights of the Layer of index "layer". The length of the
	 * outer vector must be equal to the number of nodes in the layer.
	 * The length of the inner vectors must be equal to the total count
	 * of nodes in the previous layer. Layer::SetWeights(..) throws a
	 * runtime_error if the format of the vector is invalid.
	 */
	void SetLayerWeights(int layer, vector< vector<double> > &weights);

	vector<double> Calculate(const vector<double>& input);

	void PrintInformation() const;

private:
	vector<Layer*> _layers;
};
