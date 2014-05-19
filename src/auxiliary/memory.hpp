/*
 * memory.hpp
 *
 *  Created on: Feb 27, 2013
 *      Author: steinbrf
 */

#ifndef MEMORY_HPP_
#define MEMORY_HPP_

#include <stdio.h>


long long getTotalSystemMemory();
long long getAvailableSystemMemory();

typedef struct ProcessMemoryStats_ {
	size_t size;
	size_t resident;
	size_t shared;
	size_t trs;
	size_t drs;
	size_t lrs;
	size_t dt;
	size_t pageSize;
} ProcessMemoryStats;

ProcessMemoryStats getProcessMemory();
inline void printProcessMemory(FILE *stream = stderr)
{
  ProcessMemoryStats usedMemory = getProcessMemory();
  fprintf(stream,"\nUsed Memory: %li Pages, %li Bytes/Page := %li Bytes",
  		usedMemory.size,usedMemory.pageSize,usedMemory.size*usedMemory.pageSize);
}


#endif /* MEMORY_HPP_ */
