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
#include "Util/Ops/ThreadedOps.hpp"
#include "Util/Arithm/Arithm.hpp"
#include "Util/Arithm/MemLayout.hpp"
#include <cassert>
#include <numeric>
#include <algorithm>
#include "ParallelOps.hpp"

namespace Mosse {
namespace Ut {

void ParallelOps::requestStop()
{
	for (auto op : threading.threadedOpWrappers) {
		op.requestStop();
	}
}

ParallelOps::ParallelOps(std::vector<std::reference_wrapper<Ops>> aOps, Port::Thread &aThread, ArithmBase &aArithmBase,
		MemLayoutBase &aMemLayoutBase) : ops{aOps}, threading{{}, {}}, lowLevelAtomics{{aArithmBase, aMemLayoutBase}}
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
			return Tp::NumVariant{aInit.f32 + aRhs.result().numVariant.f32};  // WARN May get broken after certain changes. See the description for `Ops`
		});

	return ret;
}

Tp::NumVariant ParallelOps::imageAbsDevLog2Sum(Tp::Image aImage, Tp::NumVariant aMean)
{
	setExec(&Ops::imageAbsDevLog2Sum, aImage, aMean);
	Tp::NumVariant ret{0.0f};
	ret = std::accumulate(threading.threadedOpWrappers.begin(), threading.threadedOpWrappers.end(), ret,
		[](const Tp::NumVariant &aInit, const ThreadedOps &aRhs)
		{
			return Tp::NumVariant{aInit.f32 + aRhs.result().numVariant.f32};  // WARN May get broken after certain changes. See the description for `Ops`
		});

	return ret;
}

void ParallelOps::imageCropPreprocessImpl(Tp::Image aImageReal, void *aBufferComplex, Tp::NumVariant aLog2Sum,
	Tp::NumVariant aAbsDevLog2Sum)
{
	setExec(&Ops::imageCropPreprocessImpl, aImageReal, aBufferComplex, aLog2Sum, aAbsDevLog2Sum);
}

void ParallelOps::maxReal(const void *aComplexBuffer, Tp::PointRowCol &aPeakPos, float *aSum)
{
	std::vector<Tp::PointRowCol> peakPos;
	std::vector<float> sum;
	peakPos.reserve(ops.size());

	// Start operations and wait for them to finish

	if (nullptr != aSum) {
		sum.reserve(ops.size());
		*aSum = 0.0f;
	}

	for (std::size_t i = 0; i < ops.size(); ++i) {
		if (nullptr == aSum) {
			threading.threadedOpWrappers[i].setExec(&Ops::maxReal, aComplexBuffer, peakPos[i], nullptr);
		} else {
			threading.threadedOpWrappers[i].setExec(&Ops::maxReal, aComplexBuffer, peakPos[i], &sum[i]);
		}
	}

	// Merge the results into one

	threading.waitDone();
	aPeakPos = *std::max_element(peakPos.begin(), peakPos.end(),
		[this, aComplexBuffer](const Tp::PointRowCol &aLargest, const Tp::PointRowCol &aCandidate)
		{
			auto largest = lowLevelAtomics.buffer.memLayout.atAsVariant(aLargest, roi(), aComplexBuffer);
			auto candidate = lowLevelAtomics.buffer.memLayout.atAsVariant(aCandidate, roi(), aComplexBuffer);

			return lowLevelAtomics.buffer.arithm.gt(candidate, largest);
		});

	if (nullptr != aSum) {
		*aSum = std::accumulate(sum.begin(), sum.end(), 0.0f);
	}
}

float ParallelOps::calcPsr(const void *aComplexBuffer, const Tp::PointRowCol &aPeak,
	float aSumHint, Tp::PointRowCol aMask)
{
	return ops[0].get().calcPsr(aComplexBuffer, aPeak, aSumHint, aMask);
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
