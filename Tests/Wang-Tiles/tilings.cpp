#include <iostream>
#include "tiles.h"
using namespace std;

const int M = 2;
const int N = 3;

using tiling = vector<vector<tile>>;

/*
*/
vector<tiling> & operator+=(std::vector<tiling>& A, const std::vector<tiling>& B)
{
	A.reserve(A.size() + B.size());
	A.insert(A.end(), B.begin(), B.end());
	return A;
}

/*
*/
vector<tiling> tilings_recursive(tiling& R, const tiles& T_row,
	const tiles& T_other, int row, int column) {
	vector<tiling> thao;
	//
	if (row == M)
		thao.push_back(R);
	else if (column == N) {
		thao += tilings_recursive(R, T_row, T_other, row + 1, 0);
		thao += tilings_recursive(R, T_other, T_row, row + 1, 0);
	}
	else {
		vector<tile> valid = T_row.get_valid_tiles(R[row][column]);
		for (tile t : valid) {
			R[row][column] = t;
			// New boundary conditions in right and lower edge
			if (row + 1 < M)
				R[row + 1][column].a = t.c;
			if (column + 1 < N)
				R[row][column + 1].b = t.d;
			thao += tilings_recursive(R, T_row, T_other, row, column + 1);
		}
	}
	return thao;
}

/*
*/
vector<tiling> tilings(tiling& R, const tiles& T2_map, const tiles& T2_3_map) {
	vector<tiling> thao;
	thao += tilings_recursive(R, T2_map, T2_3_map, 0, 0);
	thao += tilings_recursive(R, T2_3_map, T2_map, 0, 0);
	return thao;
}


int main() {
	/*
	*
	*/
	int a = 0;
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
	return 0;
}