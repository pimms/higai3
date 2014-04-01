#include "nnet.h"
#include "netparser.h"
#include <stdlib.h>
#include "trainingdata.h"


int main(int argc, char *argv[]) 
{
	try {
		srand((unsigned)time(0));

		Topology t;
		t.push_back(pair<int,int>(1, 0));
		t.push_back(pair<int,int>(5, 0));
		t.push_back(pair<int,int>(1, 0));

		MultiLayerPerceptron *mlp;
		mlp = new MultiLayerPerceptron(t);

		mlp->Train("");
		
		delete mlp;

	} catch (runtime_error err) {
		printf("Exception caught:\n\t%s\n", err.what());
		return 1;
	}

	return 0;
}
