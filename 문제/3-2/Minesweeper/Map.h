#pragma once
#include "Block.h"
#include <vector>
#define MAP_SIZE 9
#define MINE_COUNT 10
#define ENTER 13

using namespace std;
class Map
{
private:
	vector<vector<Block*>> map;
public:
	Map();
	bool push(int x, int y);
	void init();
	void print();
	~Map();
};

