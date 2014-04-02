#include "nnet.h"
#include "netparser.h"
#include "trainingparser.h"
#include <stdlib.h>


int main(int argc, char *argv[]) 
{
	try {
		//TrainingParser tparser("trainingdata.trn");
		//vector<TrainingData> td = tparser.Parse();

		srand((unsigned)time(0));

		Topology t;
		t.push_back(pair<int,int>(1, 0));
		t.push_back(pair<int,int>(2, 0));
		t.push_back(pair<int,int>(1, 0));

		NeuralNetwork *mlp;
		mlp = new NeuralNetwork(t);
		mlp->dEta = 0.5;

		const char *file = "example.data";

		for (int i=0; i<100000; i++) {
			double pre = 0.0;
			double post = 0.0;

			mlp->Test(file);
			pre = mlp->dAvgTestError;

			mlp->Train(file);

			mlp->Test(file);
			post = mlp->dAvgTestError;
			
			//printf("\nPre-error: %g\nPost-error: %g\n",
			//		pre, post);
		}
		
		delete mlp;

	} catch (runtime_error err) {
		printf("Exception caught:\n\t%s\n", err.what());
		return 1;
	}

	return 0;
}
