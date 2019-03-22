#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "allocator.hpp"

struct Position
{
	int x;
	int y;
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
	}
	for(int i = 9; i > -1; i--)
	{
		alloc.free<Position>(positions[i]);
	}
	print(alloc.freeBlocks[0].size);
}