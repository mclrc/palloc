#include <iostream>
#include <stdlib.h>

struct Allocator
{
	char* start;
	size_t size;
	size_t used;
	Allocator(size_t size)
	{
		this->size = size;
		used = 0;
		start = (char*)malloc(size);

	}
	template <typename T>
	T* allocate()
	{
		T* pointer = (T*)&start[used];
		used += sizeof(T);
		return pointer;
	}
};