#include "Map.h"

Map* Map::instance = NULL;

Map::Map() {

}

Map::~Map() {

}

void Map::LoadMap(int curStage) {
	HANDLE hFile = CreateFile(stageFile[curStage-1], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			DWORD readB;
			ReadFile(hFile, &map[i][j], sizeof(char), &readB, NULL);
		}
	}
	CloseHandle(hFile);
}

bool Map::TankCollision(RECT rect) {
	for (int i = 0; i < MAP_SIZE; i++){
		for (int j = 0; j < MAP_SIZE; j++)
		{
			if (map[i][j] == 'n' || map[i][j] == 'w' || map[i][j] == 'W' ||
				(i == 25 && j == 12) || (i == 24 && j == 12) || (i == 25 && j == 13) || (i == 24 && j== 13)) {
				RECT m_rect = { BLOCK_WIDTH * j, BLOCK_HEIGHT * i ,BLOCK_WIDTH * (j + 1), BLOCK_HEIGHT * (i + 1) };
				RECT rcTemp;
				if (IntersectRect(&rcTemp, &rect, &m_rect))
					return true;
			}
		}
	}
	return false;
}

bool Map::MissileCollisioin(RECT rect) {
	bool flag = false;
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++)
		{
			RECT m_rect = { BLOCK_WIDTH * j, BLOCK_HEIGHT * i ,BLOCK_WIDTH * (j + 1), BLOCK_HEIGHT * (i + 1) };
			RECT rcTemp;
			if (IntersectRect(&rcTemp, &rect, &m_rect)) {
				if (map[i][j] == 'n') {
					map[i][j] = 'B';
					flag = true;
				}
				else if (map[i][j] == 'w')
					flag = true;
			}
		}
	}
	return flag;
}

char Map::GetBlockInfo(int y, int x) {
	return map[y][x];
}