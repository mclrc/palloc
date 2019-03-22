#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "allocator.hpp"

struct Position
{
	int x;
	int y;
	void test()
	{
		output("methods work");
	}
};

int main()
{
	Position* positions[10] = {};
	Allocator alloc(sizeof(Position) * 10);

	for(int i = 0; i < 10; i++)
	{
		Position* pos = alloc.allocate<Position>();
		pos->x = 20;
		pos->y = 10;
		positions[i] = pos;
		positions[i]->test();
	}
	output("");
	for(int i = 9; i > -1; i--)
	{
		output(alloc.freeFragments.size()); // Stays one - free space is combined
		alloc.free<Position>(positions[i]);
	}
}