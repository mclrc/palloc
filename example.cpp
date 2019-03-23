#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "allocator.hpp"

struct Position
{
	int x;
	int y;

	Position(int x = 0, int y = 0) : x(x), y(y) {}
	~Position()
	{
		print("destructor called");
	}
};

int main()
{
	Allocator alloc(100);

	Position* pos = alloc.allocate<Position>();

	print(pos->x);
	print(pos->y);

	alloc.free<Position>(pos);

	int* test = alloc.allocate<int>(5);
	alloc.free(test, sizeof(int));

	int* testarray = (int*)alloc.allocate(sizeof(int) * 10);

	for(int i = 0; i < 10; i++)
	{
		testarray[i] = i;
		print(testarray[i]);
	}
}