//
// MallocCounter.hpp
//
// Created on: Aug 16, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(SRC_MALLOCCOUNTER_HPP_)
#define SRC_MALLOCCOUNTER_HPP_

#include <malloc.h>

class MallocCounter {
public:
	MallocCounter();
	~MallocCounter();
	static unsigned long long getPeakCount();
private:
	static void *mallocHook(size_t, const void *);
	static void freeHook(void *aPtr, const void *);
	static void *reallocHook(void *aPtr, size_t aSize, const void *);
};

#endif // SRC_MALLOCCOUNTER_HPP_
