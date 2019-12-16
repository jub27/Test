#include <stdio.h>
#include <Windows.h>
void main() {
	int num1, num2;
	scanf_s("%d %d", &num1, &num2);
	if (num1 <= num2)
		num1 = num2;
	if (num1 % 2 == 0)
		printf("%d\n", num1);
	system("pause");
}