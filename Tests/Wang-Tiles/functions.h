#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "distances.h"
using namespace std;

// Third parameter of the random_shuffle algorithm
int random_function(int j);

// Function that builds the valid_tile hash maps, one for each tile 
// set which is divided the wang tile set: T2 and T2_3.
// PARAMETERS:
//     * map: Input-output hash map, with key:tile and value:list of 
// valid tiles.
//     * tile_set: List of tiles that represents either the T2 or 
// T2_3 tile set.
//
// PROCEDURE:
//     * Iterates all tiles of the tile_set.
//     * For each tile in this set, it updates the respective 
//       hash_map in each position that can be a valid tile.
void mapping_bc_to_tiles(valid_tiles& map, const vector<tile>& tile_set);

// Function that obtains a tiling of size NxM randomly
// PARAMETERS:
//     * N: Indicates the size of the tiling.
//     * T2_map: hash map, with key:tile and value:list of 
// valid tiles from T2.
//     * T2_3_map: hash map, with key:tile and value:list of 
// valid tiles from T2_3.
// PROCEDURE:
//     * First calls the backtracking algorithm that search tilings
// in a random order, with the boolean parameter first = true, this
// tells the algorithm to stop when it finds the first valid tiling.
//     * This algorithm returns a list of valid tilings that contains only
// one tiling, which is the tiling that returns this function.
// RETURN: Returns a random valid tiling. 
tiling random_tiling(const int N, const int M, const valid_tiles& T2_map, 
	const valid_tiles& T2_3_map);

// Function used to abstract the actual recursive method that calculates
// the valid tilings.
// PARAMETERS:
//     * thao: Stores all valid tilings.
//     * T2_map: hash map, with key:tile and value:list of 
// valid tiles from T2.
//     * T2_3_map: hash map, with key:tile and value:list of 
// valid tiles from T2_3.
//     * first: Indicates the if the search of tilings stop 
// when it finds the first valid tiling.
//     * N: Number of rows of R.
// PROCEDURE:
//     * There are 2 different options, depending whether you want to 
// start by putting T2 or T2_3 tiles in the first row.
void tilings(vector<tiling>& thao, tiling& R, const valid_tiles& T2_map, 
	const valid_tiles& T2_3_map, const bool first, const int N, const int M);

// Function that choose randomly to start the next row in the backtracking
// search of valid tilings with tiles from one set or another. Note that
// in a certain row, it is not possible to have tiles from different sets.
// PARAMETERS:
//     * thao: Stores all valid tilings.
//     * R: Represents the region of tiles with a partial tiling.
//     * T2_map: hash map, with key:tile and value:list of 
// valid tiles from T2.
//     * T2_3_map: hash map, with key:tile and value:list of 
// valid tiles from T2_3.
//     * first: Indicates the if the search of tilings stop when it finds the 
// first valid tiling.
//     * found: Boolean that finishes the backtracking search of valid tilings.
//     * N: Number of rows of R.
//     * M: Number of columns of R.
// PROCEDURE:
//     * There are 2 different options, depending whether you want to 
// start by putting T2 or T2_3 tiles in the first row.
void random_choice_to_start_next_row(vector<tiling>& thao, tiling & R, const valid_tiles& T_row,
	const valid_tiles& T_other, const int next_row, const bool first, bool& found, const int N,
	const int M);

// This is the HEART of the algorithm. It contains the backtracking algorithm to obtain
// all possible tilings given a boundary condition
// on a certain region.
// PARAMETERS:
//     * thao: Stores all valid tilings.
//     * R: Represents the region of tiles with a partial tiling.
//     * T2_map: hash map, with key:tile and value:list of 
// valid tiles from T2.
//     * T2_3_map: hash map, with key:tile and value:list of 
// valid tiles from T2_3.
//     * first: Indicates the if the search of tilings stop when it finds the 
// first valid tiling.
//     * found: Boolean that finishes the backtracking search of valid tilings.
//     * N: Number of rows of R.
//     * M: Number of columns of R.
// PROCEDURE:
//     * There are 2 different options, depending whether you want to 
// start by putting T2 or T2_3 tiles in the first row.
// PROCEDURE: 
//     * Is a backtracking algorithm to search all possible tilings
//     * In each cell of R, determined for the parameter (row, column),
// we call recursively after fixing a valid tile, if the
// search finishes in that branch then it fix a different tile in that position
// and continues the search.
//     * If first = true, then the search finishes when the first tiling is
// found, otherwise it finishes when the size of thao is longer than MAX_SIZE.
void tilings_backtracking(vector<tiling>& thao, tiling R, const valid_tiles& T_row,
	const valid_tiles& T_other, const int row, const int column, const bool first,
	bool & found, const int N, const int M);


tiling set_boundary_condition(const boundary_condition & upper_bc,
	const boundary_condition& left_bc, boundary_condition& lower_bc,
	const boundary_condition& right_bc, const int N, const int M);

edge_changed random_change_bc(tiling& bc_changed, const boundary_condition& upper_bc,
	const boundary_condition& left_bc, boundary_condition& lower_bc,
	const boundary_condition& right_bc, const int N, const int M);
#endif