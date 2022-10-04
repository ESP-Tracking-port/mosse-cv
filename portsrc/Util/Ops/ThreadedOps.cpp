//
// ThreadedOps.cpp
//
// Created on: Sep 19, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Types/Tracking.hpp"
#include "Types/Repr.hpp"
#include "ThreadedOps.hpp"
#include "Port/OsApi.hpp"

namespace Mosse {
namespace Ut {

void ThreadedOps::run()
{
	mosse_assert(Port::OsApi::instance() != nullptr);
	shouldRun = true;

	while (shouldRun) {
		if (!isDone()) {
			(this->*executorCb)();
		} else {
			Port::OsApi::instance()->taskYieldMinDelay();
		}
	}
}

ThreadedOps::ThreadedOps(DecomposedOps &aOps) : storage{{0}, {0}, {0}}, ops{aOps}, executorCb{nullptr}
{
}

}  // namespace Ut
}  // namespace Mosse
