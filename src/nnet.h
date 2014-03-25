#pragma once

#include "types.h"

class Layer;


class NeuralNetwork {
public:
	NeuralNetwork(Topology topology);
	~NeuralNetwork();

	vector<double> Calculate(const vector<double>& input);

private:
	vector<Layer*> _layers;
};
