//
// MallocCounter.cpp
//
// Created on: Aug 16, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "MallocCounter.hpp"
#include <map>

static unsigned long long sPeakMallocCounter = 0;
static unsigned long long sMallocCounter = 0;
static auto sDefaultMallocHook = __malloc_hook;
static auto sDefaultFreeHook = __free_hook;
static auto sDefaultReallocHook = __realloc_hook;
std::map<void *, unsigned long long> sMallocMap;

void *MallocCounter::mallocHook(size_t aSize, const void *)
{
	__malloc_hook = sDefaultMallocHook;
	void *mem = malloc(aSize);

	if (nullptr != mem) {
		sMallocMap[mem] = aSize;
		sMallocCounter += aSize;
	}

	if (sPeakMallocCounter < sMallocCounter) {
		sPeakMallocCounter = sMallocCounter;
	}

	__malloc_hook = mallocHook;

	return mem;
}

MallocCounter::MallocCounter()
{
	__malloc_hook = mallocHook;
	__free_hook = freeHook;
	__realloc_hook = reallocHook;
}

MallocCounter::~MallocCounter()
{
	__malloc_hook = sDefaultMallocHook;
	__free_hook = sDefaultFreeHook;
	__realloc_hook = sDefaultReallocHook;
}

unsigned long long MallocCounter::getPeakCount()
{
	return sPeakMallocCounter;
}

void MallocCounter::freeHook(void *aPtr, const void *)
{
	if (aPtr != nullptr) {
		__free_hook = sDefaultFreeHook;
		free(aPtr);

		if (sMallocMap.end() != sMallocMap.find(aPtr)) {
			sMallocCounter -= sMallocMap[aPtr];
		}

		__free_hook = freeHook;
	}
}

void *MallocCounter::reallocHook(void *aPtr, size_t aSize, const void *)
{
	void *ptrNew = nullptr;
	__realloc_hook = sDefaultReallocHook;
	unsigned long long memPrev = 0;

	if (sMallocMap.end() != sMallocMap.find(aPtr)) {
		memPrev = sMallocMap[ptrNew];
	}

	ptrNew = realloc(aPtr, aSize);

	if (ptrNew != nullptr) {
		sMallocCounter -= memPrev;
		sMallocCounter += aSize;
		sMallocMap[ptrNew] = aSize;
	}

	__realloc_hook = reallocHook;

	return ptrNew;
}
