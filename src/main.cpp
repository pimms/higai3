#include "nnet.h"
#include "netparser.h"
#include "trainingparser.h"
#include <stdlib.h>
#include <ctime>

int main(int argc, char *argv[]) 
{
	try {
		//TrainingParser tparser("trainingdata.trn");
		//vector<TrainingData> td = tparser.Parse();

		srand((unsigned)time(0));
		NetworkParser parser("smallnet.nnet");
		NeuralNetwork *nn = parser.Parse();

		vector<double> input, output;
		input.push_back(1.0);
		input.push_back(1.0);

		output = nn->Propagate(input);

		for (int i=0; i<output.size(); i++) {
			printf("[Neuron %i]:  %f\n", i, output[i]);
		}

		nn->PrintInformation();

	} catch (runtime_error err) {
		printf("Exception caught:\n\t%s\n", err.what());
		return 1;
	}

	return 0;
}
