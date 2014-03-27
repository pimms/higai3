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
