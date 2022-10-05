//
// ThreadedOps.cpp
//
// Created on: Sep 19, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "embmosse/Types/Tracking.hpp"
#include "embmosse/Types/Repr.hpp"
#include "ThreadedOps.hpp"
#include "embmosse/Port/OsApi.hpp"

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
			Port::OsApi::instance()->taskYieldMinDelayWrap();
		}
	}
}

ThreadedOps::ThreadedOps(DecomposedOps &aOps) : storage{{0}, {0}, {0}}, ops{aOps}, executorCb{nullptr}
{
}

}  // namespace Ut
}  // namespace Mosse
