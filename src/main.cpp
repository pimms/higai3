#include "nnet.h"
#include "netparser.h"


int main(int argc, char *argv[]) 
{
	try {
		NetworkParser parser("smallnet.nnet");
		NeuralNetwork *nn = parser.Parse();

		vector<double> input, output;
		input.push_back(1.0);
		input.push_back(1.0);

		output = nn->Calculate(input);

		for (int i=0; i<output.size(); i++) {
			printf("[Neuron %i]:  %f\n", i, output[i]);
		}

	} catch (runtime_error err) {
		printf("Exception caught:\n\t%s\n", err.what());
		return 1;
	}

	return 0;
}
