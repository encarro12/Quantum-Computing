#include <iostream>
using namespace std;

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
	system("pause");
	return 0;
}