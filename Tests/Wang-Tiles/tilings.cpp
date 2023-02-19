#include <iostream>
#include "tiles.h"
using namespace std;

const int M = 2;
const int N = 4;

using tiling = vector<vector<tile>>;

/*
*/
void tilings_backtracking(vector<tiling> & thao, tiling & R, const tiles& T_row,
	const tiles& T_other, int row, int column) {
	vector<tile> valid = T_row.get_valid_tiles(R[row][column]);
	for (tile t : valid) {
		R[row][column] = t;
		if (row == M - 1 && column == N - 1) // It is a valid tiling
			thao.push_back(R);
		else {
			if (row + 1 < M)
				R[row + 1][column].a = t.c;
			if (column == N - 1) {
				tilings_backtracking(thao, R, T_other, T_row, row + 1, 0);
				tilings_backtracking(thao, R, T_row, T_other, row + 1, 0);
			}
			else {
				R[row][column + 1].b = t.d;
				tilings_backtracking(thao, R, T_row, T_other, row, column + 1);
			}
		}
	}
}

/*
*/
void tilings(vector<tiling> & thao, tiling& R, const tiles& T2_map, const tiles& T2_3_map) {
	tilings_backtracking(thao, R, T2_map, T2_3_map, 0, 0);
	tilings_backtracking(thao, R, T2_3_map, T2_map, 0, 0);
}

/*
* To analyze the stability of a boundary condition, this is the 
only function that has to be modified.
*/
tiling boundary_conditions() {
	tiling R;
	for (int row = 0; row < M; row++) {
		vector<tile> tmp;
		for (int column = 0; column < N; column++) {
			tmp.push_back({ INT_MAX, INT_MAX, INT_MAX, INT_MAX });
		}
		R.push_back(tmp);
	}
	return R;
}


int main() {
	/*
	*
	*/
	tiles T2_map = tiles({
		{1,-1,2,-1},
		{1,-1,1,0},
		{0,0,1,-1},
		{1,0,2,0}
		});
	/*
	*
	*/
	tiles T2_3_map = tiles({
		{2,-1.0 / 3,1,0},
		{2,0,1,1.0 / 3},
		{2,1.0 / 3,1,2.0 / 3},
		{2,1.0 / 3,2,-1.0 / 3},
		{2,2.0 / 3,2,0},
		{1,0,1,-1.0 / 3},
		{1,1.0 / 3,1,0},
		{1,2.0 / 3,1,1.0 / 3},
		{1,-1.0 / 3,0,1.0 / 3},
		{1,0,0,2.0 / 3}
		});
	/*
	*/

	/*
	*/
	tiling R = boundary_conditions();

	/*
	*/
	vector<tiling> thao;
	tilings(thao, R, T2_map, T2_3_map);

	return 0;
}