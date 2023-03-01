#ifndef _DISTANCES_H
#define _DISTANCES_H

#include "constants_and_DAE.h"

/*
Calculates the distance between tilings:

	d(R,Q) = Number of different tiles / N*M
*/
distance_st d(const tiling& R, const tiling& Q, const int N, const int M);

/*
Calculates the distance between boundary
conditions, necessary to analyze the stability of
one of them.

If thao is the respective list of valid tilings
given a certain bc1 and gamma the list for another
boundary condition bc2. The distance of two boundary
conditions bc1 and bc2 is:

	d(thao, gamma) = min({(R,Q)|R \in thao and Q \in R})
*/
distance_st d(const vector<tiling>& thao, 
	const vector<tiling>& gamma, const int N, const int M);

#endif 

