//
// ParallelOps.cpp
//
// Created on: Sep 19, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "embmosse/Types/Tracking.hpp"
#include "embmosse/Types/Repr.hpp"
#include "embmosse/Port/MossePort.hpp"
#include "embmosse/Port/Thread.hpp"
#include "embmosse/Util/Ops/ThreadedOps.hpp"
#include "embmosse/Util/Arithm/Arithm.hpp"
#include "embmosse/Util/Arithm/MemLayout.hpp"
#include "embmosse/Port/OsApi.hpp"
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

void ParallelOps::imageCropInto(Tp::Image aImageReal, void *aBufferComplex)
{
	Port::OsApi::instance()->setTaskYieldMinDelayFlag(false);  // Disable control yielding, parallel ops will be of high demand now
	DecomposedOps::imageCropInto(aImageReal, aBufferComplex);
}

/// \arg aManagedThreadId Managed thread operations will be performed in the current thread to spare context switching
/// expenses. Set it to any number higher than the index of any operation in the list, if managed thread is not needed.
///
ParallelOps::ParallelOps(std::vector<std::reference_wrapper<DecomposedOps>> aOps, Port::Thread &aThread,
	ArithmBase &aArithmBase, MemLayoutBase &aMemLayoutBase, const std::vector<float> &aSplit,
	std::size_t aManagedThreadId) :
	ops{aOps},
	threading{{}, {}, aSplit, aManagedThreadId},
	lowLevelAtomics{{aArithmBase, aMemLayoutBase}}
{
	mosse_assert(ops.size() > 0);
	mosse_assert(threading.threadedOpWrappers.size() == 0);
	mosse_assert(threading.opThreads.size() == 0);
	mosse_assert(Port::OsApi::instance() != nullptr);
	mosse_assert(std::accumulate(threading.split.begin(), threading.split.end(), 0.0f) <= 1);
	mosse_assert(aSplit.size() == 0 || aSplit.size() == ops.size());
	threading.threadedOpWrappers.reserve(ops.size());
	threading.opThreads.reserve(ops.size());
	std::size_t threadId = 0;

	for (auto op : ops) {
		threading.threadedOpWrappers.emplace_back(op);
		threading.opThreads.emplace_back(aThread.makeFromTask(threading.threadedOpWrappers.back()));

		// Start a thread, only if this is not the managed one
		if (threadId != aManagedThreadId) {
			ohdebug("ParallelOps", "starting thread", threadId);
			threading.opThreads.back()->start();
		}

		++threadId;
	}
}

void ParallelOps::imageConvFftDomain(void *aioCropFft2Complex, void *aMatrixAcomlex, void *aMatrixBcomplex)
{
	mosse_assert(nullptr != aioCropFft2Complex);
	mosse_assert(nullptr != aMatrixAcomlex);
	mosse_assert(nullptr != aMatrixBcomplex);
	setExec(&DecomposedOps::imageConvFftDomain, aioCropFft2Complex, aMatrixAcomlex, aMatrixBcomplex);
}

/// \brief Designate each Ops instance with its ROI fragment
///
void ParallelOps::initImpl()
{
	DecomposedOps::initImpl();

	for (auto op : ops) {
		op.get().init(roi());
	}

	if (isFirstInit()) {
		if (threading.split.size() != ops.size()) {  // Split load between threads equally
			const auto fragRows = roi().size(0) / ops.size();
			mosse_assert(fragRows > 0);
			// Set each `Ops` instance its ROI fragment so it can be processed in parallel fashion
			Tp::Roi frag = {{0, 0}, {fragRows, roi().size(1)}};
			{
				Tp::PointRowCol fragShift = {fragRows, 0};

				for (auto op : ops) {
					op.get().setRoiFragment(frag);
					ohdebug(parallel, "setting ROI fragment", frag);
					frag.origin += fragShift;
				}
			}

			// Handle uneven split
			{
				const auto remainder = roi().size(0) - fragRows * ops.size();
				frag.origin(0) -= static_cast<int>(fragRows);  // It's been shifted over the ROI boundary in the loop above
				frag.size(0) += static_cast<int>(remainder);
				ops[ops.size() - 1].get().setRoiFragment(frag);
			}
		} else {
			std::vector<int> roiFragmentHeights;
			const auto height = static_cast<float>(roi().size(0));
			roiFragmentHeights.reserve(threading.split.size());
			std::transform(threading.split.begin(), threading.split.end(), std::back_inserter(roiFragmentHeights),
				[height](float aSpl) {return static_cast<int>(aSpl * height); });
			roiFragmentHeights.back() += (roi().size(0) - std::accumulate(roiFragmentHeights.begin(),
				roiFragmentHeights.end(), 0));  // Handle uneven split
			int base = 0;

			for (int i = 0; i < ops.size(); ++i) {
				int height = roiFragmentHeights[i];
				ops[i].get().setRoiFragment({{base, 0}, {height, roi().size(1)}});
				base += height;
			}

			for (auto &op : ops) {
				ohdebug(split, op.get().roiFragment());
			}
		}
	}
}

void ParallelOps::fft2(void *aBufferComplex)
{
	mosse_assert(nullptr != aBufferComplex);
	ops[0].get().fft2(aBufferComplex);
}

void ParallelOps::ifft2(void *aBufferComplex)
{
	mosse_assert(nullptr != aBufferComplex);
	ops[0].get().ifft2(aBufferComplex);
}

void ParallelOps::mataUpdate(void *aMatAcomplex, const void *aImageCropFftComplex, bool aInitial)
{
	mosse_assert(nullptr != aMatAcomplex);
	mosse_assert(nullptr != aImageCropFftComplex);
	setExec(&DecomposedOps::mataUpdate, aMatAcomplex, aImageCropFftComplex, aInitial);
}

void ParallelOps::matbUpdate(void *aMatBcomplex, const void *aImageCropFftComplex, bool aInitial)
{
	mosse_assert(nullptr != aMatBcomplex);
	mosse_assert(nullptr != aImageCropFftComplex);
	setExec(&DecomposedOps::matbUpdate, aMatBcomplex, aImageCropFftComplex, aInitial);
	Port::OsApi::instance()->setTaskYieldMinDelayFlag(true);  // Restore control yielding
}

// TODO Generalize the approach beyond floats (WONTBEDONE: probably, won't be needed)
//
/// \warning While the architectural approach implies use of arbitrary types, this implementation expects FLOATS in
/// Tp::NumVariant
///
Tp::NumVariant ParallelOps::imageLog2Sum(Tp::Image aImage)
{
#if 0
	// Debug. Decomposition.
	{
		auto frag = ops[0].get().roiFragment();
		ops[0].get().setRoiFragment({{0, 0}, roiFragment().size});
		auto ret = ops[0].get().imageLog2Sum(aImage);
		ops[0].get().setRoiFragment(frag);

		return ret;
	}
#endif

	setExec(&DecomposedOps::imageLog2Sum, aImage);
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
#if 0
	// Debug. Decomposition.
	{
		auto frag = ops[0].get().roiFragment();
		ops[0].get().setRoiFragment({{0, 0}, roiFragment().size});
		auto ret = ops[0].get().imageAbsDevLog2Sum(aImage, aMean);
		ops[0].get().setRoiFragment(frag);

		return ret;
	}
#endif

	setExec(&DecomposedOps::imageAbsDevLog2Sum, aImage, aMean);
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
	mosse_assert(nullptr != aBufferComplex);
#if 0
	// Debug. Decomposition.
	{
		auto frag = ops[0].get().roiFragment();
		ops[0].get().setRoiFragment({{0, 0}, roiFragment().size});
		ops[0].get().imageCropPreprocessImpl(aImageReal, aBufferComplex, aLog2Sum, aAbsDevLog2Sum);
		ops[0].get().setRoiFragment(frag);

		return;
	}
#endif
	setExec(&DecomposedOps::imageCropPreprocessImpl, aImageReal, aBufferComplex, aLog2Sum, aAbsDevLog2Sum);
}

void ParallelOps::maxReal(const void *aComplexBuffer, Tp::PointRowCol &aPeakPos, float *aSum)
{
	mosse_assert(nullptr != aComplexBuffer);
	std::vector<Tp::PointRowCol> peakPos;
	std::vector<float> sum;
	peakPos.resize(ops.size());

	// Start operations and wait for them to finish

	if (nullptr != aSum) {
		sum.resize(ops.size(), 0.0f);
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
	auto largest = lowLevelAtomics.buffer.memLayout.atAsVariant(peakPos[0], roi(), aComplexBuffer);  // Opt.: Optimizes out unnecessary object construction
	aPeakPos = *std::max_element(peakPos.begin(), peakPos.end(),
		[this, aComplexBuffer, &largest](const Tp::PointRowCol &, const Tp::PointRowCol &aCandidate)
		{
			auto candidate = lowLevelAtomics.buffer.memLayout.atAsVariant(aCandidate, roi(), aComplexBuffer);
			bool res = lowLevelAtomics.buffer.arithm.gt(candidate, largest);

			if (res) {
				largest = candidate;
			}

			return res;
		});

	if (nullptr != aSum) {
		*aSum = std::accumulate(sum.begin(), sum.end(), 0.0f);
	}
}

float ParallelOps::calcPsr(const void *aComplexBuffer, const Tp::PointRowCol &aPeak,
	float aSumHint, Tp::PointRowCol aMask)
{
	mosse_assert(nullptr != aComplexBuffer);
	Tp::Roi roiMask{aPeak - (aMask / 2), aMask};
	roiMask.fitShift(roi().size);
	const float sizeMasked = static_cast<float>(roi().area() - roiMask.area());
	float mean = (aSumHint - ops[0].get().bufferSum(aComplexBuffer, roiMask)) / sizeMasked;

	for (std::size_t i = 0; i < ops.size(); ++i) {
		threading.threadedOpWrappers[i].setExec(&DecomposedOps::bufferAbsDevSum, aComplexBuffer,
			ops[i].get().roiFragment(), mean);
	}

	threading.waitDone();
	float devsum = std::accumulate(threading.threadedOpWrappers.begin(), threading.threadedOpWrappers.end(), 0.0f,
		[](float aAccumulated, const ThreadedOps &aOps)
		{
			return aAccumulated + aOps.result().f32;
		});
	devsum -= ops[0].get().bufferAbsDevSum(aComplexBuffer, roiMask, mean);
	float stddev = devsum / sqrtf(sizeMasked);
	float maxValue = bufferAtAsFloat(aComplexBuffer, aPeak);
	float psr = (maxValue - mean) / stddev;

	return psr;
}

float ParallelOps::bufferAtAsFloat(const void *aComplexBuffer, const Tp::PointRowCol &aPeak)
{
	mosse_assert(nullptr != aComplexBuffer);
	return ops[0].get().bufferAtAsFloat(aComplexBuffer, aPeak);
}

float ParallelOps::bufferSum(const void *aComplexBuffer, const Tp::Roi &aRoi)
{
	mosse_assert(nullptr != aComplexBuffer);
	setExec(&DecomposedOps::bufferSum, aComplexBuffer, aRoi);
	float res = std::accumulate(threading.threadedOpWrappers.begin(), threading.threadedOpWrappers.end(), 0.0f,
		[](float accumulated, const ThreadedOps &aOps)
		{
			return accumulated + aOps.result().f32;
		});

	return res;
}

float ParallelOps::bufferAbsDevSum(const void *aComplexBuffer, const Tp::Roi &aRoi, float aMean)
{
	mosse_assert(nullptr != aComplexBuffer);
	setExec(&DecomposedOps::bufferAbsDevSum, aComplexBuffer, aRoi, aMean);
	float res = std::accumulate(threading.threadedOpWrappers.begin(), threading.threadedOpWrappers.end(), 0.0f,
		[](float accumulated, const ThreadedOps &aOps)
		{
			return accumulated + aOps.result().f32;
		});

	return res;
}

void ParallelOps::Threading::waitDone()
{
	// Execute managed op, if there is one. Set its "done" flag to `true`
	if (managedThreadId < threadedOpWrappers.size()) {
		threadedOpWrappers[managedThreadId].tryIter();
	}

	for (auto &op : threadedOpWrappers) {
		while (!op.isDone()) {
			Port::OsApi::instance()->taskYieldMinDelayWrap();
		}
	}
}

}  // namespace Ut
}  // namespace Mosse
