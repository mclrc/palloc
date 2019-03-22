#include <iostream>
#include <stdlib.h>
#include <map>
#include <vector>
#define output(x) std::cout << x << "\n";

// Basic struct to describe space in memory
struct MemoryBlock
{
	void* start;
	size_t size;
};

struct Allocator
{
	// Pointer to the start of the entire memory block
	char* start;
	// Total size of the memory block
	size_t totalSize;
	// A vector of MemoryBlocks that describe the free areas of the memory block
	std::vector<MemoryBlock> freeFragments;

	Allocator(size_t size)
	{
		this->totalSize = size;
		// Actually allocate the memory on the heap
		start = (char*)malloc(size);
		// Push the first MemoryBlock to the vector describing the entire block, because its all free
		freeFragments.push_back({start, size});

	}
	template <typename T>
	T* allocate()
	{
		T* pointer = NULL;
		for(int i = 0; i < freeFragments.size(); i++)
		{
			// Check for free space in memory that is of sufficient size
			MemoryBlock& block = freeFragments[i];
			if(block.size >= sizeof(T))
			{
				// Create a pointer to the start of that space
				pointer = (T*)block.start;
				// Move the start of the space so it no longer includes the now used space
				block.start = (char*)block.start + sizeof(T);
				// Decrease the size of the space
				block.size -= sizeof(T);
				// If the space is empty, remove it from the vector
				if(block.size == 0)
				{
					freeFragments.erase(freeFragments.begin() + i);
				}
				// Break out of the loop
				break;
			}
		}
		// If no space was found, print a warning
		if(pointer == NULL)
		{
			output("[!] CRITICAL: Not enough concurrent memory left to store data of that type");
		}
		return pointer;
	}
	template <typename T>
	void free(T* pointer)
	{
		// Check if the pointer is either null or doesnt belong to this allocator
		if(pointer == NULL)
		{
			output("[!] This pointer is null");
			return;
		}
		if((char*)pointer < start || (char*)start + totalSize < (char*)pointer)// if(!((char*)start <= (char*)pointer) || (char*)start + sizeof(T) < (char*)pointer)
		{
			output("[!] This was not allocated using this allocator");
			return;
		}

		// Describe the space being freed
		MemoryBlock target = {pointer, sizeof(T)};
		// Loop to all the fragments to find the free space thats potentially in front of / behind the space we now want to free
		for(int i = 0; i < freeFragments.size(); i++)
		{
			MemoryBlock fragment = freeFragments[i];
			if((char*)fragment.start + fragment.size == (char*)target.start)
			{
				output("Preceeding space found");
				// This space directly preceeds whats being freed now
				// Put the pointer at the beginning of the preceeding space and check for any proceeding, potentially free spaces
				target.start = fragment.start;
				target.size += fragment.size;
				freeFragments.erase(freeFragments.begin() + i);
				i -= 1;

			} else if((char*)fragment.start == (char*)target.start + target.size)
			{
				output("proceeding space found");
				// This space directly proceeds whats being freed
				// Erase the proceeding block from the vector and extend the one being freed to include it
				target.size += fragment.size;
				freeFragments.erase(freeFragments.begin() + i);
				i -= 1;
				//freeFragments.push_back({pointer, sizeof(T)});
			}
		}
		freeFragments.push_back(target);
	}
};