#include <iostream>
#include <stdlib.h>
#include <map>
#include <vector>
#define print(x) std::cout << x << "\n";

struct MemoryBlock
{
	void* start;
	size_t size;
};

struct Allocator
{
	// Pointer to the start of the overall buffer
	char* start;
	// Total size of the buffer
	size_t totalSize;
	// A vector of MemoryBlocks that describe the free areas of the buffer
	std::vector<MemoryBlock> freeBlocks;

	Allocator(size_t size)
	{
		this->totalSize = size;
		// Actually allocate the memory on the heap
		start = (char*)malloc(size);
		// Push the first MemoryBlock to the vector describing the entire buffer, because its all free
		freeBlocks.push_back({start, size});

	}
	template <typename T>
	T* allocate()
	{
		T* pointer = NULL;
		for(int i = 0; i < freeBlocks.size(); i++)
		{
			// Check for free space in memory that is of sufficient size
			MemoryBlock& block = freeBlocks[i];
			if(block.size >= sizeof(T))
			{
				// Create a pointer to the start of that block
				pointer = (T*)block.start;
				// Move the start of the block so it no longer includes the now used space
				block.start = (char*)block.start + sizeof(T);
				// Decrease the size of the block
				block.size -= sizeof(T);
				// If the block is empty, remove it from the vector
				if(block.size == 0)
				{
					freeBlocks.erase(freeBlocks.begin() + i);
				}
				// Break out of the loop
				break;
			}
		}
		// If no space was found, print a warning
		if(pointer == NULL)
		{
			print("[!] CRITICAL: Not enough concurrent memory left to store data of that type");
		}
		return pointer;
	}
	template <typename T>
	void free(T* pointer)
	{
		// Check if the pointer is either null or doesnt belong to this allocator
		if(pointer == NULL)
		{
			print("[!] This pointer is null");
			return;
		}
		if((char*)pointer < start || (char*)start + totalSize < (char*)pointer)
		{
			print("[!] This was not allocated using this allocator");
			return;
		}

		// Describe the block being freed
		MemoryBlock target = {pointer, sizeof(T)};
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
		freeBlocks.push_back(target);
	}
};