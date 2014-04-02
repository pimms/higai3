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
		t.push_back(pair<int,int>(24, 0));
		t.push_back(pair<int,int>(12, 0));
		t.push_back(pair<int,int>(1, 0));

		NeuralNetwork *mlp;
		mlp = new NeuralNetwork(t);
		mlp->Run("dat/kaffe.trn", 50000);
		
		delete mlp;
	} catch (runtime_error err) {
		printf("Exception caught:\n\t%s\n", err.what());
		return 1;
	}

	return 0;
}
