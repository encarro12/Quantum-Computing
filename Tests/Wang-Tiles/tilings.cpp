#include <iostream>
#include "tiles.h"
using namespace std;

const int M = 5;
const int N = 6;

using tiling = vector<vector<tile>>;

/*
*/
void tilings_backtracking(vector<tiling> & thao, tiling R, const tiles& T_row,
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
void tilings(vector<tiling> & thao, tiling R, const tiles& T2_map, const tiles& T2_3_map) {
	tilings_backtracking(thao, R, T2_map, T2_3_map, 0, 0);
	tilings_backtracking(thao, R, T2_3_map, T2_map, 0, 0);
}

/*
* To analyze the stability of a boundary condition, this is the 
only function that has to be modified.
*/
void boundary_conditions(tiling & R, int option) {
	// if option == 0, then exhaustive search
	for (int row = 0; row < M; row++) {
		vector<tile> tmp;
		for (int column = 0; column < N; column++) {
			tmp.push_back({ INT_MAX, INT_MAX, INT_MAX, INT_MAX });
		}
		R.push_back(tmp);
	}
	// If option == 1, then fixed boundary condition for thao
	if(option == 1) {
		// Upper edge
		R[0][0].a = 1;
		R[0][1].a = 1; /**/
		R[0][2].a = 1;
		R[0][3].a = 1;
		R[0][4].a = 1;
		R[0][5].a = 1;

		// Left edge
		R[0][0].b = -1;
		R[1][0].b = -1.0 / 3;
		R[2][0].b = 0;
		R[3][0].b = -1;
		R[4][0].b = 0;

		/*
		// Lower edge
		R[4][0].c = 1;
		R[4][1].c = 0;
		R[4][2].c = 1;
		R[4][3].c = 2;
		R[4][4].c = 1;
		R[4][5].c = 1;

		// Right edge
		R[0][5].d = -1;
		R[1][5].d = 2.0/3;
		R[2][5].d = -1.0/3;
		R[3][5].d = -1;
		R[4][5].d = 2.0/3;
		*/
	}
	// If option == 1, then fixed boundary condition for gamma
	else if (option == 2) {
		// Upper edge
		R[0][0].a = 1;
		R[0][1].a = 1; /**/
		R[0][2].a = 1;
		R[0][3].a = 0;
		R[0][4].a = 1;
		R[0][5].a = 1;

		// Left edge
		R[0][0].b = -1;
		R[1][0].b = -1.0 / 3;
		R[2][0].b = 0;
		R[3][0].b = -1;
		R[4][0].b = 0;

		/*
		// Lower edge
		R[4][0].c = 1;
		R[4][1].c = 0;
		R[4][2].c = 1;
		R[4][3].c = 2;
		R[4][4].c = 1;
		R[4][5].c = 1;

		// Right edge
		R[0][5].d = -1;
		R[1][5].d = 2.0/3;
		R[2][5].d = -1.0/3;
		R[3][5].d = -1;
		R[4][5].d = 2.0/3;
		*/
	}
}

/*
*/
double d(const tiling& R, const tiling& Q) {
	double count = 0;
	for (int row = 0; row < M; row++)
		for (int column = 0; column < N; column++)
			if (!(R[row][column] == Q[row][column]))
				count++;
	return count / (N * M);
}

/*
*/
double d(const vector<tiling>& thao, const vector<tiling>& gamma) {
	double distance = INT_MAX;
	for (tiling R : thao)
		for (tiling Q : gamma)
			distance = min(distance, d(R, Q));
	return distance;
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
	tiling R, Q;
	boundary_conditions(R, 1);
	boundary_conditions(Q, 2);

	/*
	*/
	vector<tiling> thao, gamma;
	tilings(thao, R, T2_map, T2_3_map);
	tilings(gamma, Q, T2_map, T2_3_map);

	cout << "d(gamma, thao) = " << d(gamma, thao) << endl;
	return 0;
}