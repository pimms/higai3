#include "nnet.h"
#include "netparser.h"
#include <stdlib.h>
#include "trainingdata.h"


int main(int argc, char *argv[]) 
{
	try {
		srand((unsigned)time(0));

		MultiLayerPerceptron mlp();

	} catch (runtime_error err) {
		printf("Exception caught:\n\t%s\n", err.what());
		return 1;
	}

	return 0;
}
