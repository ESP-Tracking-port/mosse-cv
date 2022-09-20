//
// StlThread.hpp
//
// Created on: Sep 20, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_PORT_THREAD_STLTHREAD_HPP_)
#define MOSSE_PORT_THREAD_STLTHREAD_HPP_

#include "Port/Task.hpp"
#include "Port/Thread.hpp"
#include <thread>

namespace Mosse {
namespace Port {

class StlThread : public Thread {
public:
	std::unique_ptr<Thread> makeFromTask(Task &) override;
	virtual void start() override;
	StlThread();
private:
	StlThread(Task &aTask);
private:
	Task *task;
	std::thread thread;
};

}  // namespace Port
}  // namespace Mosse

#endif // MOSSE_PORT_THREAD_STLTHREAD_HPP_
