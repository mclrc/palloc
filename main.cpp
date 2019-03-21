#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "allocator.hpp"
#define output(x) std::cout << x << "\n";

struct Position
{
	int x;
	int y;
};

int main()
{
	// Create allocator with 50B of memory
	Allocator alloc(50);
	// Allocate space for an object of type Position
	Position* pos = alloc.allocate<Position>();
	// Read and write to that object - it works!
	pos->y = 11;
	// Allocate space for a second Position object. That works too!
	Position* pos2 = alloc.allocate<Position>();
	pos2->x = 22;

	alloc.free<Position>(pos);
	for(auto i : alloc.freeFragments)
	{
		output(i.size);
	}
	output("\n\n");
	alloc.free<Position>(pos2);
	// output(alloc.freeFragments[0].size);
	// output(alloc.freeFragments[1].size);
	for(auto i : alloc.freeFragments)
	{
		output(i.size);
	}
}