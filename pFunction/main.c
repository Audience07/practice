#include <stdio.h>
#include <stdlib.h>
int add(int a, int b);
int main(int argc, char* argv[])
{
	int(*padd)(int, int);
	padd = &add;
	printf("%d", add(3, 4));
	return 0;
}
int add(int a, int b) {
	return a + b;
}