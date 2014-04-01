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

class MultiLayerPerceptron {
public:
	MultiLayerPerceptron(Topology topology);
	~MultiLayerPerceptron();

	int Train(const char* fnames);
	int Test (const char* fname);
	int Evaluate();

	void Run(const char* fname, const int& maxiter);

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

	double dMSE;
	double dMAE;
};

