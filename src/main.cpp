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
	"	-tfile <file>\n"
	"		Define the file containing the training data\n"
	"		Passing this argument causes the program to load a text file\n"
	"		containing the training data.\n"
	"	-img [IMPLICIT BY DEFAULT]\n"
	"		Use images\n"
	"	-scalefactor [REQUIRED FOR -img]\n"
	"		The factor with which to scale down parsed images. Use 1 for\n"
	"		no change, 2 for half size, 3 for 1/3 size, etc.\n"
	"	-top <int>,<int>...<int>\n"
	"		Define the topology of the network.\n"
	"		Example: -top 1,3,1\n"
	"		\n"
	"		When using text files (-tfile), the topology will be literally\n"
	"		translated. When using images (-img), the -top argument only\n"
	"		define the hidden layers. \n"
	"		Example with scaling factor 5:\n"
	"			-top 4,3\n"
	"			will result in the topology:\n"
	"			36,4,3,26\n"
	" 	-deta <double>\n"
	"		Set the training rate of the network\n"
	"	-iter <int>\n"
	"		Set the maximum number of iterations to trai to\n"
	;


void ParseArgs(int argc, char **argv, CmdConfig *conf);
void ParseTopology(char *str, Topology *t);
void PrintHelp();
TrainingSet ParseTrainingData(CmdConfig *conf);


int main(int argc, char *argv[]) 
{
	try {
		srand((unsigned)time(0));

		Topology t;
		ResultData result;
		CmdConfig conf;

		conf.topology = &t;
		ParseArgs(argc, argv, &conf);

		TrainingSet tset = ParseTrainingData(&conf);

		NeuralNetwork mlp(t);
		mlp.dEta = conf.eta;
		mlp.Run(tset, conf.maxIterations, &result);

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
		} else if (!strcmp(argv[i], "-img")) {
			conf->img = true;
		} else if (!strcmp(argv[i], "-scalefactor")) {
			conf->scalefactor = atoi(argv[++i]);
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

	if (conf->img && conf->trainingFile) {
		printf("ERROR: Both -img and -tfile defined.\n");
		PrintHelp();
		exit(1);
	}

	if (!conf->trainingFile) {
		conf->img = true;
	}

	if (conf->img && conf->scalefactor <= 0) {
		printf("ERROR: Undefined / invalid value for scalefactor\n");
		PrintHelp();
		exit(1);
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
}

void PrintHelp()
{
	printf("%s", HELP_USAGE);
}

TrainingSet ParseTrainingData(CmdConfig *conf)
{
	TrainingSet tset;
	TrainingParser parser;

	if (conf->img) {
		tset = parser.ParseImages(conf);

		pair<int,int> inl, outl;
		inl.first = (30/conf->scalefactor) * (30/conf->scalefactor);
		outl.first = 26;

		conf->topology->insert(conf->topology->begin(), inl);
		conf->topology->push_back(outl);
	} else {
		tset = parser.ParseText(conf->trainingFile, conf->topology);
	}

	return tset;
}

