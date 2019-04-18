#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "allocator.hpp"
#define cout(x) std::cout << x << "\n";

struct Position
{
	int x;
	int y;

	Position(int x = 0, int y = 0) : x(x), y(y) {}
	~Position()
	{
		std::cout << "dest" << "\n";
	}
};

int main()
{
	Allocator alloc(100);

	Position* pos = alloc.allocate<Position>();
	alloc.free(pos);
}