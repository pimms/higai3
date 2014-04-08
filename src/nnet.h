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
	
	int Pass (const char* fname, bool train);
	int Evaluate();

	void Run(const char* fname, int maxiter, ResultData *res);

	double dEta;
	double dAlpha;
	double dGain;
	double dAvgTestError;

private:
	void RandomWeights();

	void SetInputSignal (double* input);
	void GetOutputSignal(double* output);

	void SaveWeights();
	void RestoreWeights();

	void PropagateSignal();
	void ComputeOutputError(double* target);
	void BackPropagateError();
	void AdjustWeights();

	void Simulate(double* input, double* output, 
				  double* target, bool training);

	int    nNumLayers;
	Layer* pLayers;

	double dMSE; // TODO: wtf is this
	double dMAE; // TODO: wtf is this
};

