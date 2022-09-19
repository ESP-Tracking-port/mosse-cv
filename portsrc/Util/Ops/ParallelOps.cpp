//
// ParallelOps.cpp
//
// Created on: Sep 19, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "ParallelOps.hpp"
#include <cassert>

namespace Mosse {
namespace Ut {

ParallelOps::ParallelOps(std::vector<std::reference_wrapper<Ops> > aOps) : ops{aOps}
{
	// TODO spawn threads
	// TODO assert size
}

/// \brief Designate each Ops instance with its ROI fragment to get it processed in parallel
///
void ParallelOps::initImpl()
{
	Ops::initImpl();  // TODO (XXX): Any unintended effects?
	const auto fragRows = roi().size(0) / threads;  // TODO handle uneven split
	assert(fragRows > 0);
	auto frag = roiFragment();

	// Set each `Ops` instance its ROI fragment so it can be processed in parallel fashion
	{
		const auto fragShift = {fragRows, 0};

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
