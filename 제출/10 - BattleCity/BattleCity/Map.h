#pragma once
#include <Windows.h>

#define MAP_SIZE 26

#define BLOCK_WIDTH 16
#define BLOCK_HEIGHT 13

class Map
{
private:
	LPCSTR stageFile[5] = { "stage1.txt", "stage2.txt", "stage3.txt", "stage4.txt", "stage5.txt" };

	char map[MAP_SIZE][MAP_SIZE];

	static Map* instance;
public:
	static Map* GetInstance() {
		if (instance == NULL) {
			instance = new Map();
		}
		return instance;
	}
	Map();
	void LoadMap(int curStage);
	char GetBlockInfo(int y, int x);
	bool TankCollision(RECT rect);
	bool MissileCollisioin(RECT rect);
	~Map();
};

