#pragma once

#include "types.h"


class NeuralNetwork;
class TrainingData;


/* Training Controller
 *
 * Class responsible for training the neural network.
 */
class TrainingController {
public:
	TrainingController();
	~TrainingController();
		
	void Perform(const TrainingData &tdata);
		
private:
	/* Perform a single back-propagation step.
	 */
	double Train(vector<double> input, vector<double> eout);
	
	double CalculateOutputError(vector<double> eout);
	void BackPropagateError();
	void AdjustWeights();
	
	NeuralNetwork *_nnet;
};
