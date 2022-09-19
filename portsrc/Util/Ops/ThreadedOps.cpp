//
// ThreadedOps.cpp
//
// Created on: Sep 19, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Types/Tracking.hpp"
#include "Types/Repr.hpp"
#include "ThreadedOps.hpp"

namespace Mosse {
namespace Ut {

void ThreadedOps::run()
{
	shouldRun = true;

	while (shouldRun) {
		if (isDone()) {
			(this->*executorCb)();
		}
	}
}

ThreadedOps::ThreadedOps(Ops &aOps) : storage{{0}, {0}, {0}}, ops{aOps}, executorCb{nullptr}
{
}

}  // namespace Ut
}  // namespace Mosse
