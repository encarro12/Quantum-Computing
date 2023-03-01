#ifndef _CONSTANTS_DATA_STRUCTURES_H
#define _CONSTANTS_DATA_STRUCTURES_H

#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

// --------------------------------------------------------------
//           DATA STRUCTURES USED IN THE SCRIPT
// --------------------------------------------------------------

// Structure of a certain square tile with colored edges.
struct tile {
	double a; // upper edge
	double b; // left edge
	double c; // lower edge
	double d; // right edge
	double T; // set of tiles: 2 (from T2), 2/3 (from T2_3) or INT_MAX.

	// Constructor
	tile(double a, double b, double c, double d, double T) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
		this->T = T;
	}

	// Operator used to compared tiles,
	// useful for calculate the distance
	// between tilings.
	bool operator== (const tile& other) const {
		return other.a == a &&
			other.b == b &&
			other.c == c &&
			other.d == d;
	}
};

// To evaluate efficiently which possible tiles
// can fit in a certain block of the region R, a hashmap
// is necessary.

// This hash map has as key a certain tile, that
// represents the boundary conditions of this certain
// tile. If there is not a boundary condition in
// an edge then it is represented as infinity (in
// this case INT_MAX).

// In order to use the struct tile as key for
// the hash map this hash function is necessary.
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


// Represents the region or matrix of tiles.
using tiling = vector<vector<tile>>;

// Hash map that maps a certain "boundary condition" tile
// to a list of possible valid tiles.
using valid_tiles = unordered_map<tile, vector<tile>, hash_fn>;

struct tl_bc {
	double edge = 0;
	double tile_set = 0;
};
using boundary_condition = vector<tl_bc>;

struct edge_changed {
	int row = 0, col = 0;
	string edge = "";
};

struct distance_st {
	double value = 0;
	tiling nearest = {};
};


//-----------------------------------------------------------------
//                CONSTANTS USED IN THE SCRIPT
//-----------------------------------------------------------------

// Set of tiles T2 from the Wang Tile. Note that if we 
// choose a tile from this set in a row of the tiling, then the
// row must be filled of tiles from this set.
const vector<tile> T2 = {
		{1,-1,2,-1, 2},
		{1,-1,1,0, 2},
		{0,0,1,-1, 2},
		{1,0,2,0, 2}
};
// Set of tiles T2_3 from the Wang Tile. Note that if we 
// choose a tile from this set in a row of the tiling, then the
// row must be filled of tiles from this set.
const vector<tile> T2_3 = {
		{2,-1.0 / 3,1,0, 2.0/3},
		{2,0,1,1.0 / 3, 2.0 / 3},
		{2,1.0 / 3,1,2.0 / 3, 2.0 / 3},
		{2,1.0 / 3,2,-1.0 / 3, 2.0 / 3},
		{2,2.0 / 3,2,0, 2.0 / 3},
		{1,0,1,-1.0 / 3, 2.0 / 3},
		{1,1.0 / 3,1,0, 2.0 / 3},
		{1,2.0 / 3,1,1.0 / 3, 2.0 / 3},
		{1,-1.0 / 3,0,1.0 / 3, 2.0 / 3},
		{1,0,0,2.0 / 3, 2.0 / 3}
};

// Possible values in the upper edge of the tiling, separated in each
// possible tile set T2 a T2_3.
const vector<double> UPPER = {0,1,2};
// Possible values in the left edge of the tiling, separated in each
// possible tile set T2 a T2_3.
const vector<double> LEFT = { -1,0, -1.0 / 3,0,1.0 / 3,2.0 / 3 }; // { { -1,0}, { -1.0 / 3,0,1.0 / 3,2.0 / 3, 0} };
// Possible values in the right edge of the tiling, separated in each
// possible tile set T2 a T2_3.
const vector<double> RIGHT = { -1, 0, 1.0 / 3, 2.0 / 3, -1.0 / 3 }; 
// Possible values in the lower edge of the tiling, separated in each
// possible tile set T2 a T2_3.
const vector<double> LOWER = {0,1,2};

// Start size of the square matrix or tiling
const int START_SIZE_OF_REGION = 5;
// End size of the square matrix or tiling
const int LAST_SIZE_OF_REGION = 10;

// When studying the stability of a certain tiling, we need this
// constant to determine the number of samples generated to see
// how the distance change over size.
const int SAMPLES_NUMBER = 10;

// The distance, although is O(N^4), if N is big then the
// cost of computation makes the algorithm impracticable.
// To avoid this situation we fixed this constant to 
// study only the boundary conditions that have a number of tilings <= MAX_SIZE
const int MAX_SIZE = 10000;


// --------------------------------------------------------------
//      CONSTANTS THAT DETERMINE THE NATURE OF THE STABILITY
// --------------------------------------------------------------

// In order to determine if we want to analyze either a border
// or parallel or fixed boundary conditions we need to set
// these constants.

// If true, then the script is analyzing upper boundary condition stability
const bool UPPER_BC = true;
// If true, then the script is analyzing left boundary condition stability
const bool LEFT_BC = true;
// If true, then the script is analyzing right boundary condition stability
const bool RIGHT_BC = false;
// If true, then the script is analyzing lower boundary condition stability
const bool LOWER_BC = false;

#endif