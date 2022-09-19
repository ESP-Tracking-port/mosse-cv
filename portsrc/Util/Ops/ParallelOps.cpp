//
// ParallelOps.cpp
//
// Created on: Sep 19, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Types/Tracking.hpp"
#include "Types/Repr.hpp"
#include "Port/MossePort.hpp"
#include "Port/Thread.hpp"
#include "ParallelOps.hpp"
#include "Util/Ops/ThreadedOps.hpp"
#include <cassert>

namespace Mosse {
namespace Ut {

void ParallelOps::requestStop()
{
	for (auto op : threading.threadedOpWrappers) {
		op.requestStop();
	}
}

ParallelOps::ParallelOps(std::vector<std::reference_wrapper<Ops>> aOps, Port::Thread &aThread) : ops{aOps},
	threading{{}, {}}
{
	assert(ops.size() > 0);
	threading.threadedOpWrappers.reserve(ops.size());
	threading.opThreads.reserve(ops.size());

	for (auto op : ops) {
		threading.threadedOpWrappers.emplace_back(op);
		threading.opThreads.emplace_back(aThread.makeFromTask(threading.threadedOpWrappers.back()));
		threading.opThreads.back()->start();
	}
}

/// \brief Designate each Ops instance with its ROI fragment
///
void ParallelOps::initImpl()
{
	Ops::initImpl();  // TODO (XXX): Any unintended effects?
	const auto fragRows = roi().size(0) / ops.size();
	assert(fragRows > 0);
	auto frag = roiFragment();

	for (auto op : ops) {
		op.get().init(roi());
	}

	// Set each `Ops` instance its ROI fragment so it can be processed in parallel fashion
	{
		Tp::PointRowCol fragShift = {fragRows, 0};

		for (auto op : ops) {
			op.get().setRoiFragment(frag);
			frag.origin += fragShift;
		}
	}

	// Handle uneven split
	{
		const auto remainder = roi().size(0) - fragRows * ops.size();
		frag.origin(0) -= fragRows;  // It's been shifted over the ROI boundary in the loop above
		frag.size(1) += remainder;
		ops[ops.size() - 1].get().setRoiFragment(frag);
	}
}

}  // namespace Ut
}  // namespace Mosse
