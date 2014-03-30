#include "nnet.h"
#include "netparser.h"
#include <stdlib.h>
#include "controller.h"


int main(int argc, char *argv[]) 
{
	try {
		srand((unsigned)time(0));

		TrainingController ctrl;
		ctrl.Perform();

	} catch (runtime_error err) {
		printf("Exception caught:\n\t%s\n", err.what());
		return 1;
	}

	return 0;
}
