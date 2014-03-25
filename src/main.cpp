#include "nnet.h"


int main(int argc, char *argv[]) 
{
	try {
		Topology t;
		vector<double> input, output;
		t.push_back(pair<int,int>(1,0));
		t.push_back(pair<int,int>(1,0));

		NeuralNetwork nn(t);

		for (int i=0; i<t[0].first; i++) {
			double d;
			printf("Double pls: ");
			scanf("%lf", &d);
			input.push_back(d);
		}
		
		output = nn.Calculate(input);

		for (int i=0; i<output.size(); i++) {
			printf("[Neuron %i]:  %f\n", i, output[i]);
		}

	} catch (runtime_error err) {
		printf("Exception caught:\n\t%s\n", err.what());
		return 1;
	}

	return 0;
}
