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
	Allocator alloc(50);
	strcpy(alloc.start, "hello there");
	Position* pos = alloc.allocate<Position>();
	output(pos->x);
	pos->y = 11;
	output(pos->y);
	// for(int i = 0; i < 5; i++)
	// {
	// 	std::cout << alloc.start[i] << "\n";
	// }
}