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
		if (!tryIter()) {
			Port::OsApi::instance()->taskYieldMinDelayWrap();
		}
	}
}

/// \brief Executed a currently assigned callback. Returns false, if unable to perform (i.e. no callback has been
/// assigned)
///
bool ThreadedOps::tryIter()
{
	bool ret = !isDone();

	if (ret) {
		(this->*executorCb)();
	}

	return ret;
}

ThreadedOps::ThreadedOps(DecomposedOps &aOps) : storage{{0}, {0}, {0}}, ops{aOps}, executorCb{nullptr}
{
}

}  // namespace Ut
}  // namespace Mosse
