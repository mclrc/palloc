#pragma once
#include <iostream>
#include <stdlib.h>
#include <map>
#include <vector>
#include <algorithm>
#define print(x) std::cout << x << "\n";

struct MemoryBlock
{
	MemoryBlock(void* s, size_t a) : start(s), size(a) {}
	void* start;
	size_t size;
};

struct Allocator
{
	// Pointer to the start of the overall buffer
	char* start;
	// Total size of the buffer
	size_t totalSize;
	size_t used;
	// A vector of MemoryBlocks that describe the free areas of the buffer
	std::vector<MemoryBlock> freeBlocks;
	// A vector of MemoryBlocks to keep track of allocations
	std::vector<MemoryBlock> allocations;

	Allocator(size_t size)
	{
		this->totalSize = size;
		// Actually allocate the memory on the heap
		start = (char*)malloc(size);
		// Push the first MemoryBlock to the vector describing the entire buffer, because its all free
		freeBlocks.push_back({start, size});
	}

	
	template <typename T>
	T* allocate(T data = NULL)
	{
		T* pointer = (T*)allocate(sizeof(T));
		memcpy(pointer, &data, sizeof(T));
		return pointer;
	}
	// Overload for allocating memory based on size, not data type
	void* allocate(size_t amount)
	{
		void* pointer = NULL;
		for(int i = 0; i < freeBlocks.size(); i++)
		{
			// Check for free space in memory that is of sufficient size
			MemoryBlock& block = freeBlocks[i];
			if(block.size >= amount)
			{
				// Create a pointer to the start of that block
				pointer = block.start;
				// Move the start of the block so it no longer includes the now used space
				block.start = (char*)block.start + amount;
				// Decrease the size of the block
				block.size -= amount;
				// If the block is empty, remove it from the vector
				if(block.size == 0)
				{
					freeBlocks.erase(freeBlocks.begin() + i);
				}
				used += amount;
				// Break out of the loop
				break;
			}
		}
		// If no space was found, print a warning
		if(pointer == NULL)
		{
			print("[!] CRITICAL: Not enough concurrent memory left to store this amount of data");
		}
		allocations.push_back(MemoryBlock((void*)pointer, amount));
		return pointer;
	}
	void free(void* start, size_t amount)
	{
		if(start == NULL)
		{
			print("[!] This pointer is null");
			return;
		}
		if((char*)start < start || (char*)start + totalSize < (char*)start || (char*)start + amount > this->start + totalSize)
		{
			print("[!] This was not allocated using this allocator or the specified data is incorrect");
			return;
		}
		// Describe the block being freed
		MemoryBlock target = {start, amount};
		// Loop to all the blocks to find the free block thats potentially in front of / behind the block being freed
		for(int i = 0; i < freeBlocks.size(); i++)
		{
			MemoryBlock block = freeBlocks[i];
			if((char*)block.start + block.size == (char*)target.start)
			{
				// This block directly preceeds whats being freed now
				// Put the pointer at the beginning of the preceeding block
				target.start = block.start;
				target.size += block.size;
				freeBlocks.erase(freeBlocks.begin() + i);
				i -= 1;

			} else if((char*)block.start == (char*)target.start + target.size)
			{
				// This block directly proceeds whats being freed
				// Erase the proceeding block from the vector and extend the one being freed to include it
				target.size += block.size;
				freeBlocks.erase(freeBlocks.begin() + i);
				i -= 1;
				//freeBlocks.push_back({pointer, sizeof(T)});
			}
		}
		used -= amount;
		freeBlocks.push_back(target);
		for(int i = 0; i < allocations.size(); i++)
		{
			if(allocations[i].start == start)
			{
				print("deleted allocation");
				allocations.erase(allocations.begin() + i);
			}
		}
		std::sort(freeBlocks.begin(), freeBlocks.end(), [](MemoryBlock a, MemoryBlock b){ return a.start < b.start;});
	}
	void free(void* pointer)
	{
		for(auto a : allocations)
		{
			if((void*)a.start == pointer)
			{
				free(a.start, a.size);
				return;
			}
		}
	}
};
