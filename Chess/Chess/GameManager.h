#pragma once
#include "Bitmap.h"
class GameManager
{
private:
	static GameManager* instance;
public:
	static GameManager* GetInstance() {
		if (instance == NULL)
			instance = new GameManager();
		return instance;
	}
	GameManager();
	~GameManager();
};

