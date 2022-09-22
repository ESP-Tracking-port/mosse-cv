//
// Thread.cpp
//
// Created on: Sep 22, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Thread.hpp"

namespace Mosse {
namespace Port {

Thread::Thread(Task *aTask) : mtask{aTask}
{
}

Task *Thread::task()
{
	return mtask;
}

}  // namespace Port
}  // namespace Mosse
