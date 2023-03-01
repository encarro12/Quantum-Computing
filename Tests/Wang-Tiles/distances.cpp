#include "distances.h"

distance_st d(const tiling& R, const tiling& Q, const int N, const int M) {
	double count = 0;
	for (int row = 0; row < N; row++)
		for (int column = 0; column < M; column++)
			if (!(R[row][column] == Q[row][column]))
				count++;
	return { count / (N * N), Q };
}

distance_st d(const vector<tiling>& thao, const vector<tiling>& gamma, const int N, const int M) {
	distance_st distance = { INT_MAX, {} };
	for (tiling R : thao)
		for (tiling Q : gamma) {
			auto next = d(R, Q, N, M);
			if (next.value < distance.value) distance = next;
		}
	return distance;
}