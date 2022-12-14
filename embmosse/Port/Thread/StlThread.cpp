//
// StlThread.cpp
//
// Created on: Sep 20, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "StlThread.hpp"
#include "embmosse/Port/MossePort.hpp"

namespace Mosse {
namespace Port {

std::unique_ptr<Thread> StlThread::makeFromTask(Task &aTask)
{
	return std::unique_ptr<Thread>{new StlThread{aTask}};
}

void StlThread::start()
{
	mosse_assert(nullptr != task());
	thread = std::thread(&Task::run, task());
	ohdebug(StlThread::start, "starting task");
}

StlThread::StlThread() : Thread{nullptr}, thread{}
{
}

StlThread::StlThread(Task &aTask) : Thread{&aTask}
{
}

}  // namespace Port
}  // namespace Mosse
