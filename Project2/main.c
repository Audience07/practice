#include <stdio.h>
#include <Windows.h>
#define MAXNUM 20

struct {
	char *name;
	int age;
}Person;


struct Box {
	int Hight;
	int Width;
};


int main() {
	Person.age = 10;
	Person.name = "xxx";
	printf("%s is %d years old\n", Person.name, Person.age);
	struct Box a = { 10,20 };
	printf("%d", a.Width);


	return 0;
}