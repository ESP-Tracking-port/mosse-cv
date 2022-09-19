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
#include <numeric>

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

void ParallelOps::imageConvFftDomain(void *aioCropFft2Complex, void *aMatrixAcomlex, void *aMatrixBcomplex)
{
	setExec(&Ops::imageConvFftDomain, aioCropFft2Complex, aMatrixAcomlex, aMatrixBcomplex);
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

void ParallelOps::fft2(void *aBufferComplex)
{
	ops[0].get().fft2(aBufferComplex);
}

void ParallelOps::ifft2(void *aBufferComplex)
{
	ops[0].get().ifft2(aBufferComplex);
}

void ParallelOps::mataUpdate(void *aMatAcomplex, const void *aImageCropFftComplex, bool aInitial)
{
	setExec(&Ops::mataUpdate, aMatAcomplex, aImageCropFftComplex, aInitial);
}

void ParallelOps::matbUpdate(void *aMatBcomplex, const void *aImageCropFftComplex, bool aInitial)
{
	setExec(&Ops::matbUpdate, aMatBcomplex, aImageCropFftComplex, aInitial);
}

// TODO Generalize the approach beyond floats (probably, won't be needed)
//
/// \warning While the architectural approach implies use of arbitrary types, this implementation expects FLOATS in
/// Tp::NumVariant
///
Tp::NumVariant ParallelOps::imageLog2Sum(Tp::Image aImage)
{
	setExec(&Ops::imageLog2Sum, aImage);
	Tp::NumVariant ret{0.0f};
	ret = std::accumulate(threading.threadedOpWrappers.begin(), threading.threadedOpWrappers.end(), ret,
		[](const Tp::NumVariant &aInit, const ThreadedOps &aRhs)
		{
			return Tp::NumVariant{aInit.f32 + aRhs.result().numVariant.f32};
		});

	return ret;
}

void ParallelOps::Threading::waitDone()
{
	for (auto op : threadedOpWrappers) {
		while (!op.isDone()) {
		}
	}
}

}  // namespace Ut
}  // namespace Mosse
