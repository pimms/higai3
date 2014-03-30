#include "controller.h"
#include "nnet.h"
#include "layer.h"
#include "neuron.h"
#include "util.h"
#include "netparser.h"

#include <math.h>


TrainingController::TrainingController()
	:	_nnet(NULL)
{
	NetworkParser parser("smallnet.nnet");
	_nnet = parser.Parse();
}

TrainingController::~TrainingController()
{
	if (_nnet) 
		delete _nnet;
}


void TrainingController::Perform()
{
	vector< vector<double> > ins;
	vector< vector<double> > outs;

	for (int i=0; i<100; i++) {
		vector<double> r, o;
		r.push_back(RandRange(90.0, 100.0));
		o.push_back((r[0] > 95.0) ? (1.0) : (-1.0));

		ins.push_back(r);
		outs.push_back(o);
	}

	_nnet->PrintInformation();

	double error = 1.0;
	int i=0;
	while (error > 0.01) {
		error = Train(ins[i%100], outs[i%100]);
		_nnet->PrintInformation();
		printf("Error: %g\n", error);
		i++;
	}

	_nnet->PrintInformation();
}


/* Private Methods */

double TrainingController::Train(vector<double> input,
							   vector<double> expOut)
{
	_nnet->Propagate(input);
	double generalError = 0.0;
	double errorSum = 0.0;
	double perrorSum = 0.0;
	
	for (int i=_nnet->GetLayerCount()-1; i>0; i--) {
		const Layer *player = _nnet->GetLayer(i-1);
		const Layer *layer = _nnet->GetLayer(i);

		vector<double> out; 
		layer->GetOutput(out);

		for (int j=0; j<layer->GetNodeCount(); j++) {
			Neuron *neuron = layer->GetNeuron(j);
			double localError = 0.0;

			if (i == _nnet->GetLayerCount()-1) {
				localError = (expOut[j]-out[j]) * out[j] * (1.0-out[j]);
				generalError += pow(expOut[j] - out[j], 2.0);
			} else {
				localError = out[j] * (1.0-out[j]) * perrorSum;
			}
			
			for (int k=0; k<player->GetNodeCount(); k++) {
				double in = neuron->GetInput(k);
				double udelta = /* eta */localError * in;
				neuron->UpdateWeight(k, udelta);
				errorSum += neuron->GetWeights()[k] * localError;
			}
		}

		perrorSum = errorSum;
		errorSum = 0.0;
	}

	return generalError;
}

