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
 * 		   and style of code. The core functionality is still the work
 * 		   of the original author.
 *********************************************************************/

#pragma once

#include "types.h"
#include "trainingparser.h"

struct TrainingData;

struct ItemStats {
	void Write(ofstream &file) const;
	void Recognize(char &ch, double &certainty) const;
	ItemStats() : trainingData(NULL) {}

	vector<double> initialResult;
	vector<double> finalResult;
	TrainingData *trainingData;
};

struct ResultData {
	void WriteLogFile(string logfilename) const;
	ResultData() : initialError(0), finalError(0), iterations(0), trainingPasses(0) {}

	double initialError;
	double finalError;
	int iterations;
	int trainingPasses;

	// One ItemStats object will be created for every TestData 
	// object.
	vector<ItemStats> stats;
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
	NeuralNetwork (string weightsFile);
	~NeuralNetwork ();
	
	void Run(TrainingSet &tset, int maxiter, ResultData *res);

	/* Perform a pass with the input set. The values of the layer will be 
	 * stored in rdata->stats[i]. If rdata->iterations is 0, ItemStats::initialResult
	 * will be used, ItemStats::finalResult otherwise. 
	 */
	void Test(TrainingSet &tset, ResultData *rdata);

	void LogTopology(string filename) const;

	double dEta;
	double dAlpha;
	double dGain;
	double dAvgTestError;

private:
	void LoadTopology(string filename);
	void SetTopology(Topology top);
	void DeleteLayers();

	int Pass(TrainingSet &tset, bool train, ResultData *rdata=NULL);

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

	double dMSE;
	double dMAE;

	Topology _topology;
};

