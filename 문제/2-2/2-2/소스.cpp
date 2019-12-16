#include <stdio.h>
#include <Windows.h>
void main() {
	int sub1, sub2, sub3;
	scanf_s("%d%d%d", &sub1, &sub2, &sub3);
	int subSum = sub1 + sub2 + sub3;
	double subAvg = subSum / 3;
	if (subAvg >= 90)
		printf("A\n");
	else if (subAvg >= 80)
		printf("B\n");
	else if (subAvg >= 70)
		printf("C\n");
	else if (subAvg >= 60)
		printf("D\n");
	else
		printf("F\n");
	system("pause");
}
