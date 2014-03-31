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


void TrainingController::Perform(const TrainingData &tdata)
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
	double e = 0.0;

	e = CalculateOutputError(expOut);
	BackPropagateError();
	AdjustWeights();

	return e;
}

double TrainingController::CalculateOutputError(vector<double> expOut)
{
	const int layerCount = _nnet->GetLayerCount();
	const Layer *layer;
	layer = _nnet->GetLayer(layerCount-1);
	double sqErr = 0.0;

	for (int i=0; i<layer->GetNodeCount(); i++) {
		Neuron *neuron = layer->GetNeuron(i);

		double y = neuron->GetOutput();
		double t = expOut[i];	
		double e = y * (1.0-y) * (t-y);
		neuron->SetError(e);

		sqErr += (t-y)*(t-y);
	}

	return sqErr / layer->GetNodeCount();
}

void TrainingController::BackPropagateError()
{
	const int layerCount = _nnet->GetLayerCount();
	const Layer *layer, *player;
	layer = _nnet->GetLayer(layerCount-1);

	// Calcuate the other layers
	for (int i=layerCount-2; i>=0; i--) {
		player = _nnet->GetLayer(i);
		layer = _nnet->GetLayer(i+1);
		
		for (int j=0; j<player->GetNodeCount(); j++) {
			Neuron *neuron = player->GetNeuron(j);

			double y = neuron->GetOutput();
			double E = 0.0;

			for (int k=0; k<layer->GetNodeCount(); k++) {
				Neuron *kn = layer->GetNeuron(k);
				E += kn->GetWeights()[j] * kn->GetError();
			}

			neuron->SetError(y * (1.0-y) * E);
		}
	}
}

void TrainingController::AdjustWeights()
{
	const int layerCount = _nnet->GetLayerCount();
	const Layer *layer, *player;
	layer = _nnet->GetLayer(layerCount-1);
	
	for (int i=1; i<layerCount; i++) {
		layer = _nnet->GetLayer(i);
		player = _nnet->GetLayer(i-1);

		for (int j=0; j<layer->GetNodeCount(); j++) {
			for (int k=0; k<player->GetNodeCount(); k++) {
				double y  = player->GetNeuron(k)->GetOutput();
				double e  = layer->GetNeuron(j)->GetError();
				//double dw = layer->GetNeuron(j)->GetDelta(k);
				
				double delta = y * e /* ETA */;
				layer->GetNeuron(j)->UpdateWeight(k, delta);
			}
		}
	}
}
