#include <stdio.h>

int multiply(int a, int b)
{
	return a * b;
}

int main(int argc, char **argv)
{
	int a,b;
	scanf("%d %d", &a, &b);
	printf("%d * %d = %d\n", a, b, multiply(a,b));
	return 0;
}
