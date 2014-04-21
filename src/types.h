#pragma once

#include <vector>
using std::vector;
using std::pair;

#include <string>
using std::string;

#include <stdlib.h>
#include <stdio.h>

#include <stdexcept>
using std::runtime_error;


/* Topology-type used when creating the neural network.
 * The first element of the pair is the number of real nodes.
 * The second element of the pair is the number of bias
 * nodes.
 */
typedef vector< pair<int,int> > Topology;

/* A layer's weight is defined by a two dimensional vector. The
 * vector is of size [MxN], where M is the number of nodes in the
 * current layer, and N is the number of incoming connections for
 * the nodes in the layer.
 */
typedef vector< vector<double> > LayerWeights;

typedef unsigned char uint8;



struct CmdConfig 
{
	bool img;
	int scalefactor;
	int samplesPerCharacter;

	Topology *topology;
	double eta;
	int maxIterations;
	const char *weightsFile;

	CmdConfig() 
		: 	img(true),
			scalefactor(0),
			topology(NULL),
			eta(0.25),
			maxIterations(10000),
			weightsFile(NULL),
			samplesPerCharacter(20)
	{ }
};
