/*********************************************************************
 * File  : mlp.h (renamed to nnet.h)
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

#pragma once

#include "types.h"
#include "trainingparser.h"

struct ResultData {
	double initialError;
	double finalError;
	int iterations;
	int trainingPasses;
};

struct Neuron {
	double  x;    
	double  e;    
	double* w;    
	double* dw;    
	double* wsave; 
};

struct Layer {
	int     nNumNeurons;
	Neuron* pNeurons;
};

class NeuralNetwork {
public:
	NeuralNetwork (Topology topology);
	~NeuralNetwork ();
	
	int Pass (const TrainingSet &tset, bool train);
	int Evaluate();

	void Run(const TrainingSet &tset, int maxiter, ResultData *res);

	double dEta;
	double dAlpha;
	double dGain;
	double dAvgTestError;

private:
	void RandomWeights();

	void SetInputSignal (const double* input);
	void GetOutputSignal(double* output);

	void SaveWeights();
	void RestoreWeights();

	void PropagateSignal();
	void ComputeOutputError(const double* target);
	void BackPropagateError();
	void AdjustWeights();

	void Simulate(const double* input, double* output, 
				  const double* target, bool training);

	int    nNumLayers;
	Layer* pLayers;

	double dMSE; // TODO: wtf is this
	double dMAE; // TODO: wtf is this

	Topology _topology;
};

