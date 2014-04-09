/*********************************************************************
 * File  : mlp.cpp (renamed to nnet.cpp)
 *
 * Original
 * author: Sylvain BARTHELEMY
 *         mailto:sylvain@sylbarth.com
 *         http://www.sylbarth.com
 * Date  : 2000-08
 *
 * Note  : The file has been heavily modified from it's original
 * 		   state to fit our specific requirements, both functional
 * 		   and style of code.
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "nnet.h"

void InitializeRandoms()
{
	srand( (unsigned)time( NULL ) );
}

int RandomEqualINT(int Low, int High)
{
	return rand() % (High-Low+1) + Low;
}

double RandomEqualREAL(double Low, double High)
{
	return ((double) rand() / RAND_MAX) * (High-Low) + Low;
}

bool read_number(FILE* fp, double* number)
{
	char szWord[256];
	int i = 0;
	int b;

	*number = 0.0;

	szWord[0] = '\0';
	while ( ((b=fgetc(fp))!=EOF) && (i<255) ) {
		if( (b=='.') ||
		        (b=='0') ||
		        (b=='1') ||
		        (b=='2') ||
		        (b=='3') ||
		        (b=='4') ||
		        (b=='5') ||
		        (b=='6') ||
		        (b=='7') ||
		        (b=='8') ||
		        (b=='9') ) {
			szWord[i++] = (char)b;
		} else if(i>0) break;
	}
	szWord[i] = '\0';

	if(i==0) return false;

	*number = atof(szWord);

	return true;
}



NeuralNetwork::NeuralNetwork(Topology topology)
	:	nNumLayers(0),
		pLayers(0),
		dEta(0.25),
		dAlpha(0.9),
		dGain(1.0),
		dMSE(0.0),
		dMAE(0.0),
		dAvgTestError(0.0),
		_topology(topology)
{
	/* TODO:
	 * Consider wether we need to support bias nodes or not. Currently
	 * bias nodes are not supported, but the number of bias nodes to
	 * include in each layer can be found in topology[x].second.
	 */
	int i,j;

	nNumLayers = topology.size();
	pLayers    = new Layer[nNumLayers];

	for ( i = 0; i < nNumLayers; i++ ) {
		int realNodes = topology[i].first;
		int biasNodes = topology[i].second;
		pLayers[i].nNumNeurons = realNodes;
		pLayers[i].pNeurons    = new Neuron[realNodes];

		for( j = 0; j < realNodes; j++ ) {
			pLayers[i].pNeurons[j].x  = 1.0;
			pLayers[i].pNeurons[j].e  = 0.0;
			if(i>0) {
				int prevReal = topology[i-1].first;
				int prevBias = topology[i-1].second;
				int prevCount = prevReal /* + prevBias */ ;

				pLayers[i].pNeurons[j].w     = new double[prevReal ];
				pLayers[i].pNeurons[j].dw    = new double[prevReal ];
				pLayers[i].pNeurons[j].wsave = new double[prevReal ];
			} else {
				pLayers[i].pNeurons[j].w     = NULL;
				pLayers[i].pNeurons[j].dw    = NULL;
				pLayers[i].pNeurons[j].wsave = NULL;
			}
		}

	}


}

NeuralNetwork::~NeuralNetwork()
{
	int i,j;
	for( i = 0; i < nNumLayers; i++ ) {
		if ( pLayers[i].pNeurons ) {
			for( j = 0; j < pLayers[i].nNumNeurons; j++ ) {
				if ( pLayers[i].pNeurons[j].w )
					delete[] pLayers[i].pNeurons[j].w;
				if ( pLayers[i].pNeurons[j].dw )
					delete[] pLayers[i].pNeurons[j].dw;
				if ( pLayers[i].pNeurons[j].wsave )
					delete[] pLayers[i].pNeurons[j].wsave;
			}
		}
		delete[] pLayers[i].pNeurons;
	}
	delete[] pLayers;
}

int NeuralNetwork::Pass(const TrainingSet &tset, bool train)
{
	int count = 0;
	int nbi   = 0;
	int nbt   = 0;

	const double *input;
	const double *target;
	double* output = new double[pLayers[nNumLayers-1].nNumNeurons];
	dAvgTestError = 0.0;

	for (int i=0; i<tset.size(); i++) {
		input = &(tset[i].input[0]);
		target = &(tset[i].expectedOutput[0]);
		Simulate(input, output, target, train);
		dAvgTestError += dMAE;
		count++;

		/* UNCOMMENT FOR DETAILED IN/OUT INFO
		printf("Input:");
		for (int j=0; j<tset[i].input.size(); j++) 
			printf("%0.2g ", tset[i].input[j]);
		printf("\nOutput: ");
		for (int j=0; j<tset[i].expectedOutput.size(); j++) 
			printf("%0.2g ", output[j]);
		printf("\n");
		getchar();
		*/
	}

	dAvgTestError /= count;
	delete[] output;
	return count;
}

void NeuralNetwork::Run(const TrainingSet &tset, int maxiter, 
						ResultData *res)
{
	bool   Stop = false;
	double dMinTestError = 0.0;

	res->iterations = 0;

	InitializeRandoms();
	RandomWeights();

	do {
		res->trainingPasses += Pass(tset, true);

		if(!res->iterations) {
			dMinTestError = dAvgTestError;
			res->initialError = dAvgTestError;
		}

		res->iterations++;
		res->finalError = dAvgTestError;
		printf( "%i \t TestError: %f", res->iterations, dAvgTestError);

		if ( dAvgTestError < dMinTestError) {
			printf(" -> saving weights\n");
			dMinTestError = dAvgTestError;
			SaveWeights();
		} else if (dAvgTestError > 1.5 * dMinTestError) {
			printf(" -> stopping training and restoring weights\n");
			Stop = true;
			RestoreWeights();
		} else {
			printf(" -> ok\n");
		}

	} while ( (!Stop) && (res->iterations<maxiter) );
}



/* Private Methods */
void NeuralNetwork::RandomWeights()
{
	int i,j,k;
	for( i = 1; i < nNumLayers; i++ ) {
		for( j = 0; j < pLayers[i].nNumNeurons; j++ ) {
			for ( k = 0; k < pLayers[i-1].nNumNeurons; k++ ) {
				pLayers[i].pNeurons[j].w [k]    = RandomEqualREAL(-1, 1);
				pLayers[i].pNeurons[j].dw[k]    = 0.0;
				pLayers[i].pNeurons[j].wsave[k] = 0.0;
			}
		}
	}
}

void NeuralNetwork::SetInputSignal(const double* input)
{
	int i;
	for ( i = 0; i < pLayers[0].nNumNeurons; i++ ) {
		pLayers[0].pNeurons[i].x = input[i];
	}
}

void NeuralNetwork::GetOutputSignal(double* output)
{
	int i;
	for ( i = 0; i < pLayers[nNumLayers-1].nNumNeurons; i++ ) {
		output[i] = pLayers[nNumLayers-1].pNeurons[i].x;
	}
}

void NeuralNetwork::SaveWeights()
{
	int i,j,k;
	for( i = 1; i < nNumLayers; i++ ) {
		for( j = 0; j < pLayers[i].nNumNeurons; j++ ) {
			for ( k = 0; k < pLayers[i-1].nNumNeurons; k++ ) {
				pLayers[i].pNeurons[j].wsave[k] = pLayers[i].pNeurons[j].w[k];
			}
		}
	}
}

void NeuralNetwork::RestoreWeights()
{
	int i,j,k;
	for( i = 1; i < nNumLayers; i++ ) {
		for( j = 0; j < pLayers[i].nNumNeurons; j++ ) {
			for ( k = 0; k < pLayers[i-1].nNumNeurons; k++ ) {
				pLayers[i].pNeurons[j].w[k] = pLayers[i].pNeurons[j].wsave[k];
			}
		}
	}
}

void NeuralNetwork::PropagateSignal()
{
	int i,j,k;

	for( i = 1; i < nNumLayers; i++ ) {
		for( j = 0; j < pLayers[i].nNumNeurons; j++ ) {
			double sum = 0.0;
			for ( k = 0; k < pLayers[i-1].nNumNeurons; k++ ) {
				double out = pLayers[i-1].pNeurons[k].x;
				double w   = pLayers[i  ].pNeurons[j].w[k];
				sum += w * out;
			}

			pLayers[i].pNeurons[j].x = 1.0 / (1.0 + exp(-dGain * sum));
		}
	}
}

void NeuralNetwork::ComputeOutputError(const double* target)
{
	int  i;
	dMSE = 0.0;
	dMAE = 0.0;
	for( i = 0; i < pLayers[nNumLayers-1].nNumNeurons; i++) {
		double x = pLayers[nNumLayers-1].pNeurons[i].x;
		double d = target[i] - x;
		pLayers[nNumLayers-1].pNeurons[i].e = dGain * x * (1.0 - x) * d;
		dMSE += (d * d);
		dMAE += fabs(d);
	}

	dMSE /= (double)pLayers[nNumLayers-1].nNumNeurons;
	dMAE /= (double)pLayers[nNumLayers-1].nNumNeurons;
}

void NeuralNetwork::BackPropagateError()
{
	int i,j,k;
	for( i = (nNumLayers-2); i >= 0; i-- ) {
		for( j = 0; j < pLayers[i].nNumNeurons; j++ ) {
			double x = pLayers[i].pNeurons[j].x;
			double E = 0.0;
			for ( k = 0; k < pLayers[i+1].nNumNeurons; k++ ) {
				E += pLayers[i+1].pNeurons[k].w[j] * pLayers[i+1].pNeurons[k].e;
			}
			pLayers[i].pNeurons[j].e = dGain * x * (1.0 - x) * E;
		}
	}
}

void NeuralNetwork::AdjustWeights()
{
	int i,j,k;
	for( i = 1; i < nNumLayers; i++ ) {
		for( j = 0; j < pLayers[i].nNumNeurons; j++ ) {
			for ( k = 0; k < pLayers[i-1].nNumNeurons; k++ ) {
				double x  = pLayers[i-1].pNeurons[k].x;
				double e  = pLayers[i  ].pNeurons[j].e;
				double dw = pLayers[i  ].pNeurons[j].dw[k];
				pLayers[i].pNeurons[j].w [k] += dEta * x * e + dAlpha * dw;
				pLayers[i].pNeurons[j].dw[k]  = dEta * x * e;
			}
		}
	}
}

void NeuralNetwork::Simulate(const double* input, double* output, 
							 const double* target, bool training)
{

	if(!input)  return;
	if(!target) return;

	SetInputSignal(input);
	PropagateSignal();
	if(output) GetOutputSignal(output);

	ComputeOutputError(target);

	if (training) {
	//	printf("test: %.2f %.2f %.2f = %.2f\n", 
	//		   input[0],input[1],target[0],output[0]);
	}

	if (training) {
		BackPropagateError();
		AdjustWeights();
	}
}



