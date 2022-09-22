//
// PthreadThread.cpp
//
// Created on: Sep 22, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Port/Task.hpp"
#include "Port/MossePort.hpp"
#if !MOSSE_PORTABLE
# include <cassert>
#endif
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
	ohdebug(PthreadThread::start);
	int s = pthread_attr_init(&attr);
	assert(0 == s);
	void *threadArg = static_cast<void *>(this);
	s = pthread_create(&threadId, &attr, threadTask, threadArg);
	assert(0 == s);
#endif
}

}  // namespace Port
}  // namespace Mosse
