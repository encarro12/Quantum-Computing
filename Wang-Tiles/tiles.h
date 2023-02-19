#ifndef _TILES_H
#define _TILES_H

#include <unordered_map>
#include <vector>

#define clave first
#define valor second

struct tile {
	int a; // upper edge
	int b; // left edge
	int c; // lower edge
	int d; // right edge

	bool operator= (const tile& other) {
		return other.a == a &&
			   other.b == b &&
			   other.c == c &&
			   other.d == d
	}
};

/*
* 
*/
class tiles {
private:
	/*
	* 
	*/
	std::unordered_map<tile, vector<tile>> map;
public:
	tiles(const vector<tile>& tile_set) {
		for (tile t : tile_set) {
			map[{t.a, INT_MAX, INT_MAX, INT_MAX}].pushback(t);
			map[{t.a, t.b, INT_MAX, INT_MAX}].pushback(t);
			map[{t.a, t.b, t.c, INT_MAX}].pushback(t);
			map[{t.a, t.b, t.c, t.d}].pushback(t);
			map[{t.a, t.b, INT_MAX, t.d}].pushback(t);
			map[{t.a, INT_MAX, t.c, INT_MAX}].pushback(t);
			map[{t.a, INT_MAX, t.c, t.d}].pushback(t);
			map[{t.a, INT_MAX, INT_MAX, t.d}].pushback(t);
			map[{INT_MAX, t.b, INT_MAX, INT_MAX}].pushback(t);
			map[{INT_MAX, t.b, t.c, INT_MAX}].pushback(t);
			map[{INT_MAX, t.b, t.c, t.d}].pushback(t);
			map[{INT_MAX, t.b, INT_MAX, t.d}].pushback(t);
			map[{INT_MAX, INT_MAX, t.c, INT_MAX}].pushback(t);
			map[{INT_MAX, INT_MAX, t.c, t.d}].pushback(t);
			map[{INT_MAX, INT_MAX, INT_MAX, t.d}].pushback(t);
			map[{INT_MAX, INT_MAX, INT_MAX, INT_MAX}].pushback(t);
		}
	}

	



};

#endif