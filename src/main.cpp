#include "nnet.h"
#include "netparser.h"
#include "trainingparser.h"
#include <stdlib.h>
#include <string.h>
#include <cstring>


const char *HELP_USAGE = 
	"Usage:\n"
	"	-h \n"
	"		Display this message\n"
	"	-tfile <file> [REQUIRED]\n"
	"		Define the file containing the training data\n"
	"	-top <int>,<int>...<int>\n"
	"		Define the topology of the network.\n"
	"		Example: -top 1,3,1\n"
	" 	-deta <double>\n"
	"		Set the training rate of the network\n"
	"	-iter <int>\n"
	"		Set the maximum number of iterations to trai to\n"
	;

struct CmdConfig {
	Topology *topology;
	double eta;
	int maxIterations;
	const char *trainingFile;
};

void ParseArgs(int argc, char **argv, CmdConfig *conf);
void ParseTopology(char *str, Topology *t);
void PrintHelp();


int main(int argc, char *argv[]) 
{
	try {
		srand((unsigned)time(0));

		Topology t;
		ResultData result;
		CmdConfig conf = {&t, 0.25, 10000, NULL};
		ParseArgs(argc, argv, &conf);

		if (!conf.trainingFile)
			throw runtime_error("Training file required (see help output)");

		NeuralNetwork mlp(t);
		mlp.dEta = conf.eta;
		mlp.Run(conf.trainingFile, conf.maxIterations, &result);

		printf("Initial error:   %g\n", result.initialError);
		printf("Final error:     %g\n", result.finalError);
		printf("Total passes:    %i\n", result.trainingPasses);
	} catch (runtime_error err) {
		printf("Exception caught:\n\t%s\n", err.what());
		return 1;
	}

	return 0;
}



void ParseArgs(int argc, char **argv, CmdConfig *conf)
{
	for (int i=1; i<argc; i++) {
		if (!strcmp(argv[i], "-deta")) {
			conf->eta = atof(argv[++i]);
		} else if (!strcmp(argv[i], "-top")) {
			ParseTopology(argv[++i], conf->topology);
		} else if (!strcmp(argv[i], "-iter")) {
			conf->maxIterations = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-tfile")) {
			conf->trainingFile = argv[++i];
		} else {
			if (strcmp(argv[i], "-h")) {
				printf("Unknown argument: \"%s\"\n", argv[i]);
			}
			PrintHelp();
			exit(0);
		}
	}
}

void ParseTopology(char *str, Topology *t) 
{
	t->clear();
	char *tok = strtok(str, ",");

	while (tok) {
		int n = atoi(tok);
		if (!n) throw runtime_error("Invalid topology value");
		
		t->push_back(pair<int,int>(n,0));
		tok = strtok(NULL, ",");
	}

	if (t->size() < 2) 
		throw runtime_error("Topology must be at least 2");
}

void PrintHelp()
{
	printf("%s", HELP_USAGE);
}

