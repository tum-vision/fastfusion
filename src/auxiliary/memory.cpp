/*
 * memory.cpp
 *
 *  Created on: Feb 27, 2013
 *      Author: steinbrf
 */

#include "memory.hpp"
#include <unistd.h>
#include <fstream>
#include <string>
long long getTotalSystemMemory()
{
    long long pages = sysconf(_SC_PHYS_PAGES);
    long long page_size = sysconf(_SC_PAGE_SIZE);
    return pages * page_size;
}

long long getAvailableSystemMemory()
{
    long long pages = sysconf(_SC_AVPHYS_PAGES);
    long long page_size = sysconf(_SC_PAGE_SIZE);
    return pages * page_size;
}

ProcessMemoryStats getProcessMemory(){
	std::fstream file("/proc/self/statm");
	ProcessMemoryStats result;
	file >> result.size;
	file >> result.resident;
	file >> result.shared;
	file >> result.trs;
	file >> result.drs;
	file >> result.lrs;
	file >> result.dt;
	result.pageSize = sysconf(_SC_PAGE_SIZE);
	return result;
}



