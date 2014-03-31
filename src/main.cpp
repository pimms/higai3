#include "nnet.h"
#include "netparser.h"
#include <stdlib.h>
<<<<<<< HEAD
#include <ctime>
=======
#include "controller.h"
#include "trainingdata.h"

>>>>>>> real stupid

int main(int argc, char *argv[]) 
{
	try {
		srand((unsigned)time(0));

		TrainingData tdata;
		TrainingController ctrl;
		ctrl.Perform(tdata);
	} catch (runtime_error err) {
		printf("Exception caught:\n\t%s\n", err.what());
		return 1;
	}

	return 0;
}
