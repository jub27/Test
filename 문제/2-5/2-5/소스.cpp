#include <Windows.h>
#include <stdio.h>
#include <time.h>

void main() {
	srand(time(NULL));
	int num;
	int answer = rand() % 100;
	
	while (1) {
		system("cls");
		printf("숫자입력 : ");
		scanf_s("%d", &num);
		if (num > answer)
			printf("down\n");
		else if (num < answer)
			printf("up\n");
		else {
			printf("맞췄습니다\n");
			break;
		}
		system("pause");
	}
	system("pause");
}