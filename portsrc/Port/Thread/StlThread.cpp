//
// StlThread.cpp
//
// Created on: Sep 20, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "StlThread.hpp"
#include <cassert>

namespace Mosse {
namespace Port {

std::unique_ptr<Thread> StlThread::makeFromTask(Task &aTask)
{
	return std::unique_ptr<Thread>{new StlThread{aTask}};
}

void StlThread::start()
{
	assert(nullptr != task);
	thread = std::thread(&Task::run, task);
}

StlThread::StlThread() : task{nullptr}, thread{}
{
}

StlThread::StlThread(Task &aTask) : task{&aTask}
{
}

}  // namespace Port
}  // namespace Mosse
