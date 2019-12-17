#include <iostream>
#include <time.h>
#include "Map.h"
#include <conio.h>

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	srand(time(NULL));
	Map* map = new Map();
	map->init();
	map->print();
	int cur_y = 0, cur_x = 0;
	while (1) {
		if (_kbhit()) {
			char c = _getch();
			switch (c) {
			case 'w':
				cur_y--;
				if (cur_y < 0)
					cur_y = 0;
				break;
			case 'a':
				cur_x--;
				if (cur_x < 0)
					cur_x = 0;
				break;
			case 's':
				cur_y++;
				if (cur_y >= MAP_SIZE)
					cur_y = MAP_SIZE - 1;
				break;
			case 'd':
				cur_x++;
				if (cur_x >= MAP_SIZE)
					cur_x = MAP_SIZE - 1;
				break;
			case ENTER:
				if (map->push(cur_x, cur_y) == false) {
					gotoxy(0, MAP_SIZE);
					cout << "GAME OVER" << endl;
					system("pause");
					delete map;
					return 0;
				}
			}
			gotoxy(cur_x * 2, cur_y);
		}
	}
	return 0;
}