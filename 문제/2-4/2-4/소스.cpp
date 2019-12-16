#include <stdio.h>
#include <Windows.h>
void main() {
	int num;
	int sum = 0;
	do {
		scanf_s("%d", &num);
		sum += num;
	} while (num != 0);
	printf("%d\n", sum);
	system("pause");
}