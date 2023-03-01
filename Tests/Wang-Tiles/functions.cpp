#include "functions.h"
#include <algorithm>
using namespace std;

int random_function(int j) {
	return rand() % j;
}

void mapping_bc_to_tiles(valid_tiles& map, const vector<tile>& tile_set) {
	// For each tile, we insert it in every possible entrance of the map
	// which represents the possible combinations of edges that the tile 
	// satisfies. 
	for (const tile & tl : tile_set) {
		map[{tl.a, INT_MAX, INT_MAX, INT_MAX, INT_MAX}].push_back(tl);
		map[{tl.a, tl.b, INT_MAX, INT_MAX, INT_MAX}].push_back(tl);
		map[{tl.a, tl.b, tl.c, INT_MAX, INT_MAX}].push_back(tl);
		map[{tl.a, tl.b, tl.c, tl.d, INT_MAX}].push_back(tl);
		map[{tl.a, tl.b, INT_MAX, tl.d, INT_MAX}].push_back(tl);
		map[{tl.a, INT_MAX, tl.c, INT_MAX, INT_MAX}].push_back(tl);
		map[{tl.a, INT_MAX, tl.c, tl.d, INT_MAX}].push_back(tl);
		map[{tl.a, INT_MAX, INT_MAX, tl.d, INT_MAX}].push_back(tl);
		map[{INT_MAX, tl.b, INT_MAX, INT_MAX, INT_MAX}].push_back(tl);
		map[{INT_MAX, tl.b, tl.c, INT_MAX, INT_MAX}].push_back(tl);
		map[{INT_MAX, tl.b, tl.c, tl.d, INT_MAX}].push_back(tl);
		map[{INT_MAX, tl.b, INT_MAX, tl.d, INT_MAX}].push_back(tl);
		map[{INT_MAX, INT_MAX, tl.c, INT_MAX, INT_MAX}].push_back(tl);
		map[{INT_MAX, INT_MAX, tl.c, tl.d, INT_MAX}].push_back(tl);
		map[{INT_MAX, INT_MAX, INT_MAX, tl.d, INT_MAX}].push_back(tl);
		map[{INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX}].push_back(tl);
	}
}

tiling random_tiling(const int N, const int M, const valid_tiles& T2_map, const valid_tiles& T2_3_map) {
	// Initialize the region without boundary conditions (all edges of 
	// every tile are INT_MAX). Note that our goal is to find a random 
	// tiling of size NxM, not find the list of valid tilings given a 
	// boundary condition.
	vector<vector<tile>> empty_bc(N, vector<tile>(M, 
		{INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX}));
	// Obtain the list of tiling that contains only the first tiling found
	// randomly
	vector<tiling> thao; tilings(thao, empty_bc, T2_map, T2_3_map, true, N, M);
	// Returns the first and unique element of thao
	return thao[0];
}

void tilings(vector<tiling>& thao, tiling& R, const valid_tiles& T2_map,
	const valid_tiles& T2_3_map, const bool first, const int N, const int M) {
	// Initialize the boolean parameter found that finishes the backtracking
	// search of valid tilings.
	bool found = false;
	// The search is random either way because that makes the search easier.
	// Note that we are not going to allow search of set of tilings longer that
	// MAX_SIZE.
	random_choice_to_start_next_row(thao, R, T2_map, T2_3_map, 0, first, found, N, M);
}

void random_choice_to_start_next_row(vector<tiling>& thao, tiling & R, const valid_tiles& T_row,
	const valid_tiles& T_other, const int next_row, const bool first, bool& found, const int N,
	const int M) {
	if (rand() % 2) { // Start with set of tiles from T_row.
		tilings_backtracking(thao, R, T_row, T_other, next_row, 0, first, found, N, M);
		tilings_backtracking(thao, R, T_other, T_row, next_row, 0, first, found, N, M);
	}
	else { // Start with set of tiles from T_other
		tilings_backtracking(thao, R, T_other, T_row, next_row, 0, first, found, N, M);
		tilings_backtracking(thao, R, T_row, T_other, next_row, 0, first, found, N, M);
	}
}

void tilings_backtracking(vector<tiling>& thao, tiling R, const valid_tiles& T_row,
	const valid_tiles& T_other, const int row, const int column, const bool first,
	bool & found, const int N, const int M) {
	// Obtain the possible tiles that fit the edges in R[row][column], i.e., satisfies
	// the boundary conditions, and if the set is not empty it shuffles to make the search
	// random. In the backtracking scheme this is PREPARING THE TOUR.
	vector<tile> possible_tiles;
	if (T_row.count(R[row][column]) != 0) {
		possible_tiles = T_row.at(R[row][column]);
		random_shuffle(possible_tiles.begin(), possible_tiles.end(), random_function);
	}
	// For each possible tile we fix it and change the edges for the right and lower tiles
	// of R and we expand the search if it is not completed, in this case the search is over
	// in that branch.
	for (const tile& tl : possible_tiles) {
		// If the found parameter is true, then the search is over.
		if (!found) {
			// If found is false, then we fix tl in (row, column) and we expand the search.
			R[row][column] = tl;
			// We fix the tile tl in the cell positionated in (row,column) in the region R
			// and then we expand the search for that branch.

			// If the region R is completed, then R represents a valid tiling and we insert
			// it in thao.
			if (row == N - 1 && column == M - 1) {
				thao.push_back(R);
				// If first = true or the size of thao is longer than MAX_SIZE, then the search
				// is over.
				found = first || (thao.size() > MAX_SIZE);
			}
			// Otherwise, we expand the search for that branch
			else {
				// If there is a row beneath then it sets the edges from the tile underneath tl,
				// where its a edge must be c edge of tl.
				if (row < N - 1) R[row + 1][column].a = tl.c;
				// If this is the last column, then the solutions must be taken alternating T_row
				// with T_other or keeping it the same way.
				if (column == M - 1) random_choice_to_start_next_row(thao, R, T_row, T_other,
					row + 1, first, found, N, M);
				// If this is not the last column, we must stay with the tiles of T_row, i.e.,
				// we must not swith T_row with T_other. Besides, we need to fix the edges
				// for the tile right next to tl, where its b edge must be d edge of tl.
				else {
					R[row][column + 1].b = tl.d; 
					tilings_backtracking(thao, R, T_row, T_other, row, column + 1, first, found, N, M);
				}
			}
		}
	}
}

tiling set_boundary_condition(const boundary_condition& upper_bc,
	const boundary_condition& left_bc, boundary_condition& lower_bc,
	const boundary_condition& right_bc, const int N, const int M) {
	vector<vector<tile>> bc(N, vector<tile>(M, { INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX }));
	// Initialize conditions
	for (int i = 0; i < N; i++) {
		if (UPPER_BC) bc[0][i].a = upper_bc[i].edge;
		if (LEFT_BC) bc[i][0].b = left_bc[i].edge;
		if (RIGHT_BC) bc[i][N - 1].d = right_bc[i].edge;
		if (LOWER_BC) bc[N - 1][i].c = lower_bc[i].edge;
	}
	return bc;
}

edge_changed random_change_bc(tiling & bc_changed, const boundary_condition& upper_bc,
	const boundary_condition& left_bc, boundary_condition& lower_bc,
	const boundary_condition& right_bc, const int N, const int M) {

	edge_changed changed;
	bool left = (rand() % 2) == 0; 
	int random_row, random_column;
	vector<double> tile_set;
	double next;

	if (LEFT_BC && left) {
		// Obtain the random row we are going to change
		random_row = rand() % N;
		// Change the edge to a different color
		do { next = LEFT[rand() % LEFT.size()]; } while (next == bc_changed[random_row][0].b);
		bc_changed[random_row][0].b = next;
		// Update the result
		changed = { random_row, 0, "left" };
	}
	// If boundary conditions are parallel wise
	if (RIGHT_BC && !left) {
		// Obtain the random row we are going to change
		random_row = rand() % N;
		// Change the edge to a different color
		do { next = RIGHT[rand() % RIGHT.size()]; } while (next == bc_changed[random_row][N-1].d);
		bc_changed[random_row][N - 1].d = next;
		// Update the result
		changed = { random_row, N - 1, "right" };
	}
	// If boundary conditions are border wise
	if (UPPER_BC && !left) {
		// Obtain the random row we are going to change
		random_column = rand() % M;
		// Change the edge to a different color
		do { next = UPPER[rand() % UPPER.size()]; } while (next == bc_changed[0][random_column].a);
		bc_changed[0][random_column].a = next;
		// Update the result
		changed = { 0, random_column, "upper" };
	}
	return changed;
}
