#pragma once

#include "types.h"


class NeuralNetwork;


/* Training Controller
 *
 * Class responsible for training the neural network.
 */
class TrainingController {
public:
	TrainingController();
	~TrainingController();
		
	void Perform();
		
private:
	/* Perform a single back-propagation step.
	 */
	double Train(vector<double> input, vector<double> eout);
	
	NeuralNetwork *_nnet;
};
