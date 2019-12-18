#include "Map.h"

Map::Map()
{
	curBlockCount = MAP_SIZE * MAP_SIZE;
}

bool  Map::push(int x, int y) {
	if (map[x][y]->getType() == "mine") {
		map[x][y]->push();
		return false;
	}
	if (map[x][y]->isPushed())
		return true;
	map[x][y]->push();
	curBlockCount--;
	if (map[x][y]->isZeroBlock()) {
		if (x > 0)
			if (map[x - 1][y]->getType() == "normal")
				push(x - 1, y);
		if (y > 0)
			if (map[x][y - 1]->getType() == "normal")
				push(x, y - 1);
		if (x > 0 && y > 0)
			if(map[x - 1][y - 1]->getType() == "normal")
				push(x - 1, y - 1);
		if (x < MAP_SIZE - 1)
			if (map[x + 1][y]->getType() == "normal")
				push(x + 1, y);
		if (y < MAP_SIZE - 1)
			if (map[x][y + 1]->getType() == "normal")
				push(x, y + 1);
		if (x < MAP_SIZE - 1 && y < MAP_SIZE - 1)
			if (map[x + 1][y + 1]->getType() == "normal")
				push(x + 1, y + 1);
		if (x > 0 && y < MAP_SIZE - 1)
			if (map[x - 1][y + 1]->getType() == "normal")
				push(x - 1, y + 1);
		if (x < MAP_SIZE - 1 && y > 0)
			if (map[x + 1][y - 1]->getType() == "normal")
				push(x + 1, y - 1);
	}
	return true;
}
void Map::init() {
	for (int i = 0; i < MAP_SIZE; i++) {
		vector<Block*> temp;
		map.push_back(temp);
		for (int j = 0; j < MAP_SIZE; j++) {
			map[i].push_back(new NormalBlock(i, j));
		}
	}
	int curMine = 0;
	do {
		int x = rand() % MAP_SIZE;
		int y = rand() % MAP_SIZE;
		if (map[x][y]->getType() == "normal") {
			delete map[x][y];
			map[x][y] = new MineBlock(x, y);
			if (x > 0)
				map[x - 1][y]->addNearMineCount();
			if (y > 0)
				map[x][y - 1]->addNearMineCount();
			if ( x > 0 && y > 0)
				map[x-1][y - 1]->addNearMineCount();
			if (x < MAP_SIZE - 1)
				map[x + 1][y]->addNearMineCount();
			if ( y < MAP_SIZE - 1)
				map[x][y + 1]->addNearMineCount();
			if( x < MAP_SIZE -1 && y < MAP_SIZE - 1)
				map[x + 1][y + 1]->addNearMineCount();
			if( x > 0 && y < MAP_SIZE - 1)
				map[x - 1][y + 1]->addNearMineCount();
			if (x < MAP_SIZE - 1 && y > 0)
				map[x + 1][y - 1]->addNearMineCount();
			curMine++;
		}
	} while (curMine < MINE_COUNT);
}
void Map::print() {
	for (int i = 0; i < MAP_SIZE; i++)
		for (int j = 0; j < MAP_SIZE; j++)
			map[i][j]->print();
}

bool Map::isClear() {
	return curBlockCount == MINE_COUNT;
}

Map::~Map()
{
	for (int i = 0; i < MAP_SIZE; i++)
		for (int j = 0; j < MAP_SIZE; j++)
			delete map[i][j];
}
