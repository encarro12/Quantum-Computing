#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;

/*
Constant that determines if the aim
is to find a possible tiling or to 
analyze the stability of a certain
boundary condition.

- search = true -> To find a possible
tiling, i.e. exhaustive search.
- search = false -> To analyze the
stability of a certain boundary condition.
*/
const bool search = false;
/*
Constants that set the size of the
tiling: M x N.
*/
const int M = 20;
const int N = 30;
const int P = 10;

/*
Structure of a certain square tile with
colored edges.
*/
struct tile {
	double a; // upper edge
	double b; // left edge
	double c; // lower edge
	double d; // right edge

	// Constructor
	tile(double a, double b, double c, double d) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
	}
	
	/*
	Operator used to compared tiles,
	useful for calculate the distance
	between tilings.
	*/
	bool operator== (const tile& other) const {
		return other.a == a &&
			other.b == b &&
			other.c == c &&
			other.d == d;
	}
};

/*
To evaluate efficiently which possible tiles 
can fit in a certain block of the region R, a hashmap 
is necessary.

This hash map has as key a certain tile, that 
represents the boundary conditions of this certain
tile. If there is not a boundary condition in 
an edge then it is represented as infinity (in
this case INT_MAX).

In order to use the struct tile as key for 
the hash map this hash function is necessary.
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

/*
Alias to simplify the data structures.
	- tiling: Represents the region or 
	matrix of tiles.
	- valid_tiles: Hash map that maps a
	certain "boundary condition" tile
	to a list of possible valid tiles. 
*/
using tiling = vector<vector<tile>>;
using valid_tiles = unordered_map<tile, vector<tile>, hash_fn>;

/*
Function that builds the valid_tile hash maps,
one for each tile set which is divided the
wang tile set: T2 and T2_3.

Parameters:
	- map: Input-output hash map,
	with key:tile and value:list of valid tiles.
	- tile_set: List of tiles that represents 
	either the T2 or T2_3 tile set.
Procedure:
	- Iterates all tiles of the tile_set.
	- For each tile in this set, it updates
	the respective hash_map in each position
	that can be a valid tile.
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
When the search boolean is true, this function
is helpful to know the boundary conditions
of the valid tilings obtained.
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
void analyze_boundary_condition(tiling R, const valid_tiles& T_map_1,
	const valid_tiles& T_map_2) {
	// Initialize upper and left boundary conditions
	vector<double> upper_bc, left_bc;
	for (int i = 0; i < N; i++) {
		upper_bc.push_back(R[0][i].a);
		left_bc.push_back(R[i][0].b);
	}
	// Initialize tiling R to get only the boundary conditions
	// and get the corresponding set thao.
	vector<tiling> thao;
	boundary_conditions(R, false, upper_bc, left_bc);
	tilings(thao, R, T_map_1, T_map_2);

	for (int sample = 0; sample < P; sample++) {
		// Initializes boundary conditions
		tiling Q;
		boundary_conditions(Q, true, upper_bc, left_bc);
		// Change the boundary condition randomly
		// TODO
	}
}

/*
This is the heart of this script. 

It contains the backtracking algorithm to obtain
all possible tilings given a boundary condition
on a certain region.

Parameters:
	- thao: Stores all valid tilings.
	- R: Stores the actual partial solution,
	it will become a valid tiling when the 
	algorithm reaches the last block in R, i.e.
	it places the last tile.
	- T_row: Hash map described before that 
	is used in the row in which the algorithm 
	is running.
	- T_other: It is possible to switch it with
	T_row when the algorithm moves to the next
	row.
	- (row, column): Position of the algorithm
	or the partial solution R.
*/
void tilings_backtracking(vector<tiling> & thao, tiling R, const valid_tiles& T_row,
	const valid_tiles& T_other, const int row, const int column, const bool first) {
	vector<tile> valid;
	// Obtain the valid tiles given a boundary condition and then shuffle them randomly
	if (T_row.count(R[row][column]) != 0) {
		valid = T_row.at(R[row][column]);
		random_shuffle(valid.begin(), valid.end());
	}
	for (tile t : valid) {
		R[row][column] = t;
		// If it is a solution, then it is stored in thao
		if (row == M - 1 && column == N - 1) {
			if (first)
				analyze_boundary_condition(R, T_row, T_other);
		}
		// Otherwise
		else {
			// If there is a row beneath then it sets the boundary condition
			if (row + 1 < M)
				R[row + 1][column].a = t.c;
			// If this is the last column in the row then 
			// the solutions must be taken alternating
			// T_row with T_other or keeping it the same way.
			if (column == N - 1) {
				tilings_backtracking(thao, R, T_other, T_row, row + 1, 0);
				tilings_backtracking(thao, R, T_row, T_other, row + 1, 0);
			}
			// Otherwise, T_row must not swith with T_other
			else {
				// Set the boundary condition for the tile located to its right.
				R[row][column + 1].b = t.d;
				tilings_backtracking(thao, R, T_row, T_other, row, column + 1);
			}
		}
	}
}

/*
Function used to abstract the actual recursive method
that calculates the valid tilings.

Note that there are 2 different calls, depending
whether you want to start by putting T2 or T3 tiles
in the first row.
*/
void tilings(vector<tiling> & thao, tiling & R, const valid_tiles& T2_map, const valid_tiles& T2_3_map) {
	tilings_backtracking(thao, R, T2_map, T2_3_map, 0, 0, true);
	tilings_backtracking(thao, R, T2_3_map, T2_map, 0, 0, true);
}

/*
To initiliaze the tiling R, i.e. to set the 
boundary conditions.

Parameters:
	- R: Tiling that will be modified
	to set the boundary conditions.
	- upper, left, lower, right: List
	of double that represents the colours
	of the boundary condition, each one
	on its respective side of the region R.
*/
void boundary_conditions(tiling & R, const bool analyze, const vector<double> & upper,
	const vector<double> & left) {
	// Initialize boundary conditions
	for (int row = 0; row < M; row++) {
		vector<tile> tmp;
		for (int column = 0; column < N; column++) {
			tmp.push_back({ INT_MAX, INT_MAX, INT_MAX, INT_MAX });
		}
		R.push_back(tmp);
	}
	// If the aim of the test is to analyze
	// the boundary condition, then:
	if (analyze) {
		// Upper and left edge conditions
		for (int i = 0; i < N; i++) {
			R[0][i].a = upper[i];
			R[i][0].b = left[i];
		}
	}
}

/*
Calculates the distance between tilings.

d(R,Q) = Number of different tiles / N*M
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
Calculates the distance between boundary
conditions, necessary to analyze the stability of
one of them.

If thao is the respective list of valid tilings
given a certain bc1 and gamma the list for another
boundary condition bc2. The distance of two boundary 
conditions bc1 and bc2 is:

d(thao, gamma) = min({(R,Q)|R \in thao and Q \in R})
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
	Builds the hash_map that maps boundary
	conditions to a list of possible 
	tiles.
	*/
	valid_tiles T2_map, T2_3_map;
	mapping_bc_to_tiles(T2_map,{
		{1,-1,2,-1},
		{1,-1,1,0},
		{0,0,1,-1},
		{1,0,2,0}
	});
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

	for (int i = 0; i < 10; i++) {
		vector<tile> tmp = T2_map.at({ 1, INT_MAX, INT_MAX, INT_MAX });
		random_shuffle(tmp.begin(), tmp.end());
		for (tile t : tmp) {
			cout << t.a << " " << t.b << " " << t.c << " " << t.d << endl;
		}
		cout << endl;
	}
	/*
	Initializes the boundary conditions
	*/
	tiling R, Q;
	boundary_conditions(R, true,
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }, // Upper edge
		{ -1,-1.0/3,0,-1,2.0/3,0,-1,2.0/3,0,-1,1.0/3,-1,1.0/3,0,-1,1.0/3,0,-1,0,-1 },	// Left edge
		{ 1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2 }, // Lower edge
		{ -1, 2.0 / 3, 0, -1, 1.0 / 3, 1.0 / 3, -1, 1.0 / 3, -1.0 / 3, -1, 1.0 / 3, -1, 2.0 / 3, -1.0 / 3, -1, 2.0 / 3, 0, 0, 2.0 / 3, -1} // Right edge
	);
	boundary_conditions(Q, true,
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }, // Upper edge
		{ -1,-1.0 / 3,0,-1,2.0 / 3,0,-1,2.0 / 3,0,-1,1.0 / 3,-1,1.0 / 3,0,-1,1.0 / 3,0,-1,0,-1 },	// Left edge
		{ 1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2 }, // Lower edge
		{ -1, 2.0 / 3, 0, -1, 1.0 / 3, 1.0 / 3, -1, 1.0 / 3, -1.0 / 3, -1, 1.0 / 3, -1, 2.0 / 3, -1.0 / 3, -1, 2.0 / 3, 0, 0, 2.0 / 3, -1 } // Right edge
	);

	/*
	Obtains the list of all valid tilings.
	*/
	vector<tiling> thao, gamma;
	tilings(thao, R, T2_map, T2_3_map);
	tilings(gamma, Q, T2_map, T2_3_map);

	/*
	Shows the distance between the boundary conditions.
	*/
	cout << "d(gamma, thao) = " << d(gamma, thao) << endl;
	return 0;
}