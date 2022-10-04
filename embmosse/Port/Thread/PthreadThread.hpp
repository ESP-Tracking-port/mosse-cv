//
// PthreadThread.hpp
//
// Created on: Sep 22, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_PORT_THREAD_PTHREADTHREAD_HPP_)
#define MOSSE_PORT_THREAD_PTHREADTHREAD_HPP_

#include "embmosse/Port/Thread.hpp"
#if !MOSSE_PORTABLE
# include <pthread.h>
#endif

namespace Mosse {
namespace Port {

class PthreadThread : public Thread {
public:
	std::unique_ptr<Thread> makeFromTask(Task &) override;
	void start() override;
	using Thread::Thread;
private:
	static void *threadTask(void *);
private:
	// TODO: teardown
#if !MOSSE_PORTABLE
	pthread_attr_t attr;
	pthread_t threadId;
	cpu_set_t cpuSet;
#endif
};

}  // namespace Port
}  // namespace Mosse

#endif // MOSSE_PORT_THREAD_PTHREADTHREAD_HPP_
