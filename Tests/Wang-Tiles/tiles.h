#ifndef _TILES_H
#define _TILES_H

#include <unordered_map>
#include <vector>
using namespace std;

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

/*
*
*/
class tiles {
private:
	/*
	*
	*/
	unordered_map<tile, vector<tile>, hash_fn> map;
public:
	tiles(const vector<tile>& tile_set) {
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

	vector<tile> get_valid_tiles(const tile & t) const {
		vector<tile> result;
		if (map.count(t) != 0)
			result = map.at(t);
		return result;
	}
};

#endif