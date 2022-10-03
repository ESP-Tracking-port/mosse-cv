//
// PthreadThread.cpp
//
// Created on: Sep 22, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Port/Task.hpp"
#include "Port/MossePort.hpp"
#include "PthreadThread.hpp"

namespace Mosse {
namespace Port {

void *PthreadThread::threadTask(void *aThread)
{
	PthreadThread &thread = *reinterpret_cast<PthreadThread *>(aThread);
	thread.task()->run();

	return nullptr;
}

std::unique_ptr<Thread> PthreadThread::makeFromTask(Task &aTask)
{
	return makeUnique<PthreadThread>(aTask);
}

void PthreadThread::start()
{
#if !MOSSE_PORTABLE
	static constexpr std::size_t knCores = 2;
	static std::size_t currentCore = 0;
	int s = 0;
	s = pthread_attr_init(&attr);
	mosse_assert(0 == s);
	CPU_ZERO(&cpuSet);
	CPU_SET(currentCore, &cpuSet);
	s = pthread_attr_setaffinity_np(&attr, sizeof(cpuSet), &cpuSet);
	mosse_assert(0 == s);
	ohdebug(PthreadThread::start);
	void *threadArg = static_cast<void *>(this);
	s = pthread_create(&threadId, &attr, threadTask, threadArg);
	mosse_assert(0 == s);
	s = pthread_setaffinity_np(threadId, sizeof(cpuSet), &cpuSet);
	mosse_assert(0 == s);
	{
		cpu_set_t cpuSetReference;
		mosse_assert(0 == pthread_getaffinity_np(threadId, sizeof(cpuSetReference), &cpuSetReference));
		mosse_assert(CPU_EQUAL(&cpuSetReference, &cpuSet));
	}
	currentCore = (currentCore + 1) % knCores;
#endif
}

}  // namespace Port
}  // namespace Mosse
