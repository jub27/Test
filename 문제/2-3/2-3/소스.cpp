#include <stdio.h>
#include <Windows.h>
void main() {
	int sum = 0;
	for (int i = 1; i <= 1000; i++) {
		if (i % 3 == 0) {
			if (i % 5 == 0)
				sum += i;
		}
		else {
			sum += i;
		}
	}
	printf("%d\n", sum);
	system("pause");
}