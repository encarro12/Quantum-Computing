#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

/*
*/
const bool search = false;
/*
*/
const int M = 20;
const int N = 30;

/*
*
*/
struct tile {
	double a; // upper edge
	double b; // left edge
	double c; // lower edge
	double d; // right edge

	tile(double a, double b, double c, double d) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
	}

	bool operator== (const tile& other) const {
		return other.a == a &&
			other.b == b &&
			other.c == c &&
			other.d == d;
	}
};

/*
*
*/
struct hash_fn
{
	std::size_t operator() (const tile& t) const
	{
		std::size_t h1 = std::hash<double>()(t.a);
		std::size_t h2 = std::hash<double>()(t.b);
		std::size_t h3 = std::hash<double>()(t.c);
		std::size_t h4 = std::hash<double>()(t.d);

		return h1 ^ h2 ^ h3 ^ h4;
	}
};

using tiling = vector<vector<tile>>;
using valid_tiles = unordered_map<tile, vector<tile>, hash_fn>;

/*
*/
void mapping_bc_to_tiles(valid_tiles& map, const vector<tile>& tile_set) {
	for (tile t : tile_set) {
		map[{t.a, INT_MAX, INT_MAX, INT_MAX}].push_back(t);
		map[{t.a, t.b, INT_MAX, INT_MAX}].push_back(t);
		map[{t.a, t.b, t.c, INT_MAX}].push_back(t);
		map[{t.a, t.b, t.c, t.d}].push_back(t);
		map[{t.a, t.b, INT_MAX, t.d}].push_back(t);
		map[{t.a, INT_MAX, t.c, INT_MAX}].push_back(t);
		map[{t.a, INT_MAX, t.c, t.d}].push_back(t);
		map[{t.a, INT_MAX, INT_MAX, t.d}].push_back(t);
		map[{INT_MAX, t.b, INT_MAX, INT_MAX}].push_back(t);
		map[{INT_MAX, t.b, t.c, INT_MAX}].push_back(t);
		map[{INT_MAX, t.b, t.c, t.d}].push_back(t);
		map[{INT_MAX, t.b, INT_MAX, t.d}].push_back(t);
		map[{INT_MAX, INT_MAX, t.c, INT_MAX}].push_back(t);
		map[{INT_MAX, INT_MAX, t.c, t.d}].push_back(t);
		map[{INT_MAX, INT_MAX, INT_MAX, t.d}].push_back(t);
		map[{INT_MAX, INT_MAX, INT_MAX, INT_MAX}].push_back(t);
	}
}

/*
*/
void represent_bc(const tiling& R) {
	// Upper edge
	cout << "{ ";
	for (int column = 0; column < N - 1; column++) {
		cout << R[0][column].a << ",";
	}
	cout << R[0][N - 1].a << " }\n";
	// Left edge
	cout << "{ ";
	for (int row = 0; row < M - 1; row++) {
		cout << R[row][0].b << ",";
	}
	cout << R[M - 1][0].b << " }\n";
}

/*
*/
void tilings_backtracking(vector<tiling> & thao, tiling R, const valid_tiles& T_row,
	const valid_tiles& T_other, int row, int column) {
	vector<tile> valid;
	if (T_row.count(R[row][column]) != 0)
		valid = T_row.at(R[row][column]);
	for (tile t : valid) {
		R[row][column] = t;
		if (row == M - 1 && column == N - 1) {
			if (search) represent_bc(R);
			thao.push_back(R);
		}
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
void tilings(vector<tiling> & thao, tiling R, const valid_tiles& T2_map, const valid_tiles& T2_3_map) {
	tilings_backtracking(thao, R, T2_map, T2_3_map, 0, 0);
	tilings_backtracking(thao, R, T2_3_map, T2_map, 0, 0);
}

/*
*/
void boundary_conditions(tiling & R, bool search, const vector<double> & upper,
	const vector<double> & left, const vector<double> & lower, const vector<double> & right) {
	// Initialize boundary conditions
	for (int row = 0; row < M; row++) {
		vector<tile> tmp;
		for (int column = 0; column < N; column++) {
			tmp.push_back({ INT_MAX, INT_MAX, INT_MAX, INT_MAX });
		}
		R.push_back(tmp);
	}
	if (!search) {
		// Upper and Lower edge conditions
		for (int column = 0; column < N; column++) {
			R[0][column].a = upper[column];
			R[M - 1][column].c = lower[column];
		}
		// Left and right edge conditions
		for (int row = 0; row < M; row++) {
			R[row][0].b = left[row];
			R[row][N - 1].d = right[row];
		}
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
	valid_tiles T2_map, T2_3_map;
	mapping_bc_to_tiles(T2_map,{
		{1,-1,2,-1},
		{1,-1,1,0},
		{0,0,1,-1},
		{1,0,2,0}
	});
	/*
	*
	*/
	mapping_bc_to_tiles(T2_3_map,{
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
	tiling R, Q;
	boundary_conditions(R, search,
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }, // Upper edge
		{ -1,-1.0/3,0,-1,2.0/3,0,-1,2.0/3,0,-1,1.0/3,-1,1.0/3,0,-1,1.0/3,0,-1,0,-1 },	// Left edge
		{ 1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2 }, // Lower edge
		{ -1, 2.0 / 3, 0, -1, 1.0 / 3, 1.0 / 3, -1, 1.0 / 3, -1.0 / 3, -1, 1.0 / 3, -1, 2.0 / 3, -1.0 / 3, -1, 2.0 / 3, 0, 0, 2.0 / 3, -1} // Right edge
	);
	boundary_conditions(Q, search,
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }, // Upper edge
		{ -1,-1.0 / 3,0,-1,2.0 / 3,0,-1,2.0 / 3,0,-1,1.0 / 3,-1,1.0 / 3,0,-1,1.0 / 3,0,-1,0,-1 },	// Left edge
		{ 1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2 }, // Lower edge
		{ -1, 2.0 / 3, 0, -1, 1.0 / 3, 1.0 / 3, -1, 1.0 / 3, -1.0 / 3, -1, 1.0 / 3, -1, 2.0 / 3, -1.0 / 3, -1, 2.0 / 3, 0, 0, 2.0 / 3, -1 } // Right edge
	);

	/*
	*/
	vector<tiling> thao, gamma;
	tilings(thao, R, T2_map, T2_3_map);
	tilings(gamma, Q, T2_map, T2_3_map);

	cout << "d(gamma, thao) = " << d(gamma, thao) << endl;
	return 0;
}