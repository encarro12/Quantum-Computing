#include <iostream>
#include <fstream>
#include "functions.h"
using namespace std;

const unordered_map<double, string> TO_STRING = {
	{0, "+0/1"},
	{1, "+1/1"},
	{-1, "-1/1"},
	{2, "+2/1"},
	{1.0 / 3, "+1/3"},
	{-1.0 / 3, "-1/3"},
	{2.0 / 3, "+2/3"}
};

void represent_tiling(const tiling& R, const double distance, ofstream& representations,
	const int N, const int M) {
	representations << "Size : " << N << " x " << N << ", Distance: " << distance << endl;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			representations << "    " << TO_STRING.at(R[i][j].a) << "    ";
		}
		representations << endl;
		for (int j = 0; j < M; j++) {
			representations << TO_STRING.at(R[i][j].b) << "    " << TO_STRING.at(R[i][j].d);
		}
		representations << endl;
		for (int j = 0; j < N; j++) {
			representations << "    " << TO_STRING.at(R[i][j].c) << "    ";
		}
		representations << endl;
	}

	representations << "-----------------------------------------------------" << endl;
}

int main() {
	// To create random numbers we need to initialize srand()
	// As a seed we take the current time in the computer.
	srand(time(NULL));

	// To store the results for further analysis we have 
	// 2 files:
	//		* distances: Is a csv file that stores the size and distance
	// from the original tiling that it is obtained randomly.
	//		* representations: Stores the different tilings obtained in the process
	// to have a visualization of the results.
	ofstream distances, representations;
	distances.open(".\\Results\\stability_of_wang_tiles_distances.csv");
	representations.open(".\\Results\\stability_of_wang_tiles_representations.txt");
	distances << "size, distance" << endl;

	// The first step consists in building the hash maps that will help
	// to filter the possible tiles that fit the boundary condition
	// at a certain lattice point on a tiling. It reduces
	// the computational cost in the backtracking algorithm
	valid_tiles T2_map, T2_3_map;
	mapping_bc_to_tiles(T2_map, T2);
	mapping_bc_to_tiles(T2_3_map, T2_3);


	for (int N = START_SIZE_OF_REGION; N <= LAST_SIZE_OF_REGION; N++) {
		// First, we obtain a tiling of size NxN randomly
		tiling R = random_tiling(N, N, T2_map, T2_3_map);
		// Then, we represent this tiling in the representation txt
		represent_tiling(R, 0, representations, N, N);
		// After that, we store the boundary conditions from this
		// tiling. Note that we are only going to use the ones
		// selected by the <EDGE>_BC constant defined in constants.h
		boundary_condition upper_bc, left_bc, lower_bc, right_bc;
		for (int i = 0; i < N; i++) {
			upper_bc.push_back({ R[0][i].a, R[0][i].T });
			left_bc.push_back({ R[i][0].b, R[i][0].T });
			lower_bc.push_back({ R[N - 1][i].c, R[N - 1][i].T });
			right_bc.push_back({ R[i][N - 1].d, R[i][N - 1].T });
		}
		// Obtain the list of tilings that satisfy the boundary conditions
		tiling bc = set_boundary_condition(upper_bc, left_bc, right_bc, lower_bc, N, N);
		vector<tiling> thao;
		do {
			vector<tiling> thao_tmp;
			tilings(thao_tmp, bc, T2_map, T2_3_map, false, N, N);
			thao = thao_tmp;
		} while (thao.size() > MAX_SIZE);
		// Finally, we analyze the stability of the tiling obtained
		// by changing a random edge of the boundary condition as 
		// many times as marked by the constant SAMPLES_NUMBER
		distance_st dst; vector<tiling> gamma; edge_changed ec;
		for (int sample = 0; sample < SAMPLES_NUMBER; sample++) {
			do {
				// We change the boundary condition randomly from bc
				vector<vector<tile>> bc_changed(bc);
				ec = random_change_bc(bc_changed, upper_bc, left_bc, right_bc, lower_bc, N, N);
				// Obtain the list of tilings for that bc changed
				vector<tiling> gamma_tmp; tilings(gamma_tmp, bc_changed, T2_map, T2_3_map, false, N, N);
				gamma = gamma_tmp;
			} while (gamma.empty() || gamma.size() > MAX_SIZE);
			// Obtain the distance of bc to bc_changed, which is the distance
			// from thao to gamma.
			dst = d(thao, gamma, N, N);
			representations << "Changed: (" << ec.row << "," << ec.col << ") " << ec.edge << ", ";
			represent_tiling(dst.nearest, dst.value, representations, N, N);
			distances << N << "," << dst.value << endl;
		}
	}

	return 0;
}