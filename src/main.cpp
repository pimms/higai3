#include "nnet.h"
#include "netparser.h"
#include "trainingparser.h"
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <ctime>
#include <math.h>


const char *HELP_USAGE = 
	"Usage:\n"
	"	-h \n"
	"		Display this message\n"
	"	-scalefactor [REQUIRED]\n"
	"		The factor with which to scale down parsed images. Use 1 for\n"
	"		no change, 2 for half size, 3 for 1/3 size, etc.\n"
	"		The parameter MUST be a factor in 30.\n"
	"	-top <int>,<int>...<int>\n"
	"		Define the topology of the network.\n"
	"		Example with scaling factor 5:\n"
	"			-top 4,3\n"
	"			will result in the topology:\n"
	"			36,4,3,26\n"
	" 	-deta <double>\n"
	"		Set the training rate of the network\n"
	"	-iter <int>\n"
	"		Set the maximum number of iterations to train to. Defaults to 10K.\n"
	"	-samples <int>\n"
	"		Set the number of images to train with per character. Defaults to 20.\n"
	"		All images will however be tested against.\n"
	;


void ParseArgs(int argc, char **argv, CmdConfig *conf);
void ParseTopology(char *str, Topology *t);
int GetScaleFactor(const char *weightsFile);
void PrintHelp();
void WinGetch();
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

		NeuralNetwork *network = NULL;
		if (conf.weightsFile)
			network = new NeuralNetwork(conf.weightsFile);
		else
			network = new NeuralNetwork(*conf.topology);

		network->dEta = conf.eta;
		
		network->Test(tset, &result);
		network->Run(tset, conf.maxIterations, &result);
		network->Test(tset, &result);

		if (result.iterations) {
			printf("Initial error:   %g\n", result.initialError);
			printf("Final error:     %g\n", result.finalError);
			printf("Total passes:    %i\n", result.trainingPasses);
		}

		result.WriteLogFile("log.txt");
		network->LogTopology("top.txt");
	} catch (runtime_error err) {
		printf("\n\nException caught:\n\t%s\n", err.what());
		WinGetch();
		return 1;
	}

	WinGetch();
	return 0;
}



void ParseArgs(int argc, char **argv, CmdConfig *conf)
{
	bool topDefined = false;
	bool scaleDefined = false;

	for (int i=1; i<argc; i++) {
		if (!strcmp(argv[i], "-deta")) {
			conf->eta = atof(argv[++i]);
		} else if (!strcmp(argv[i], "-scalefactor")) {
			conf->scalefactor = atoi(argv[++i]);
			scaleDefined = true;
		} else if (!strcmp(argv[i], "-top")) {
			ParseTopology(argv[++i], conf->topology);
			topDefined = true;
		} else if (!strcmp(argv[i], "-iter")) {
			conf->maxIterations = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-samples")) {
			conf->samplesPerCharacter = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-w")) {
			conf->weightsFile = argv[++i];
		} else {
			if (strcmp(argv[i], "-h")) {
				printf("Unknown argument: \"%s\"\n", argv[i]);
			}
			PrintHelp();
			exit(0);
		}
	}

	if (topDefined && conf->weightsFile) {
		printf("ERROR: Both -top and -w defined. They are exclusive.\n");
		PrintHelp();
		exit(1);
	}

	if (conf->img && conf->scalefactor <= 0 && !conf->weightsFile) {
		printf("ERROR: Undefined / invalid value for scalefactor.\n");
		PrintHelp();
		exit(1);
	}

	if (conf->img && scaleDefined && conf->weightsFile) {
		printf("ERROR: Both scale and weightsfile is defined.\n");
		PrintHelp();
		exit(1);
	}

	if (!conf->img && conf->topology->size() < 2 && !conf->weightsFile) {
		printf("ERROR: Topology must have at least 2 layers.\n");
		PrintHelp();
		exit(1);
	}

	if (conf->samplesPerCharacter < 1 || conf->samplesPerCharacter > 20) {
		printf("ERROR: Samples cannot exceed 20 and must be at least 1.\n");
		PrintHelp();
		exit(1);
	}

	if (conf->weightsFile) {
		conf->scalefactor = GetScaleFactor(conf->weightsFile);
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

int GetScaleFactor(const char *weightsfile)
{
	ifstream file(weightsfile);
	if (!file.is_open()) {
		printf("ERROR: Could not open weightsfile for reading\n");
		exit(1);
	}

	int layers, nodes;
	file >> layers >> nodes;

	// Round the squareroot to int
	double s = sqrt(nodes);
	return (int)s + 0.5;
}

void PrintHelp()
{
	printf("%s", HELP_USAGE);\
	WinGetch();
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
		throw runtime_error("conf->img should NEVER be false.");
	}

	return tset;
}

void WinGetch()
{
#ifdef WIN32
	getchar();
#endif
}
