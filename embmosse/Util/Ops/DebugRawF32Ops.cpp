//
// DebugRawF32Ops.cpp
//
// Created on: Sep 09, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <Fft.h>
#include "embmosse/Types/Tracking.hpp"
#include "embmosse/Types/Repr.hpp"
#include "embmosse/Port/Thread.hpp"
#include "embmosse/Port/Thread/StlThread.hpp"
#include "embmosse/Util/Ops/OpencvNativeRawF32Ops.hpp"
#include "embmosse/Util/Ops/RawF32Ops.hpp"
#include "embmosse/Util/Ops/ParallelOps.hpp"
#include <functional>
#include "DebugRawF32Ops.hpp"

namespace Mosse {
namespace Ut {

DebugRawF32Ops::DebugRawF32Ops() : wrapped{{}, {}, {{}, {}, {}}}
{
	ohdebug(DebugRawF32Ops(CONSTRUCTOR), wrapped.rawF32Ops.instanceNum, wrapped.parallelOps.th1.instanceNum,
		wrapped.parallelOps.th2.instanceNum);
	static Ut::Arithm<RawF32Ops::reprFlags.buffer> arithmOpsBuffer;
	static Ut::MemLayout<RawF32Ops::reprFlags.buffer> memLayoutOpsBuffer;
	static Port::StlThread stlThread;
	wrapped.parallelOps.ops = std::unique_ptr<ParallelOps>{
		new ParallelOps{{std::ref<DecomposedOps>(wrapped.parallelOps.th1),
		std::ref<DecomposedOps>(wrapped.parallelOps.th2)}, stlThread, arithmOpsBuffer, memLayoutOpsBuffer}};
}

void DebugRawF32Ops::imageCropInto(const Tp::Image &aImageReal, void *aBufferComplex)
{
#if 0
	wrapped.opencvNativeRawF32Ops.imageCropInto(aImageReal, aBufferComplex);
#elif 1
	wrapped.parallelOps.ops->imageCropInto(aImageReal, aBufferComplex);
#else
	wrapped.rawF32Ops.imageCropInto(aImageReal, aBufferComplex);
#endif
}

void DebugRawF32Ops::imagePreprocess(void *aCropComplex)
{
#if 0
	wrapped.opencvNativeRawF32Ops.imagePreprocess(aCropComplex);
#elif 1
	wrapped.parallelOps.ops->imagePreprocess(aCropComplex);
#else
	wrapped.rawF32Ops.imagePreprocess(aCropComplex);
#endif
}

void DebugRawF32Ops::imageConvFftDomain(void *aioCropFft2Complex, void *aMatrixAcomlex, void *aMatrixBcomplex)
{
#if 0
	wrapped.opencvNativeRawF32Ops.imageConvFftDomain(aioCropFft2Complex, aMatrixAcomlex, aMatrixBcomplex);
#elif 1
	wrapped.parallelOps.ops->imageConvFftDomain(aioCropFft2Complex, aMatrixAcomlex, aMatrixBcomplex);
#else
	wrapped.rawF32Ops.imageConvFftDomain(aioCropFft2Complex, aMatrixAcomlex, aMatrixBcomplex);
#endif
}

void DebugRawF32Ops::fft2(void *aBufferComplex)
{
#if 0
	wrapped.opencvNativeRawF32Ops.fft2(aBufferComplex);
#elif 1
	wrapped.parallelOps.ops->fft2(aBufferComplex);
#else
	wrapped.rawF32Ops.fft2(aBufferComplex);
#endif
}

void DebugRawF32Ops::ifft2(void *aBufferComplex)
{
#if 0
	wrapped.opencvNativeRawF32Ops.ifft2(aBufferComplex);
#elif 1
	wrapped.parallelOps.ops->ifft2(aBufferComplex);
#else
	wrapped.rawF32Ops.ifft2(aBufferComplex);
#endif
}

void DebugRawF32Ops::maxReal(const void *aBufferComplex, Tp::PointRowCol &aPeakPos, float *sum)
{
#if 0
	wrapped.opencvNativeRawF32Ops.maxReal(aBufferComplex, aPeakPos, sum);
#elif 1
	wrapped.parallelOps.ops->maxReal(aBufferComplex, aPeakPos, sum);
#else
	wrapped.rawF32Ops.maxReal(aBufferComplex, aPeakPos, sum);
#endif
}

float DebugRawF32Ops::calcPsr(const void *aBufferComplex, const Tp::PointRowCol &aPeak, float sumHint, Tp::PointRowCol aMask)
{
#if 0
	return wrapped.opencvNativeRawF32Ops.calcPsr(aBufferComplex, aPeak, sumHint, aMask);
#elif 1
	return wrapped.parallelOps.ops->calcPsr(aBufferComplex, aPeak, sumHint, aMask);
#else
	return wrapped.rawF32Ops.calcPsr(aBufferComplex, aPeak, sumHint, aMask);
#endif
}

void DebugRawF32Ops::mataUpdate(void *aMatAcomplex, const void *aImageCropFftComplex, bool aInitial)
{
#if 0
	wrapped.opencvNativeRawF32Ops.mataUpdate(aMatAcomplex, aImageCropFftComplex, aInitial);
#elif 1
	wrapped.parallelOps.ops->mataUpdate(aMatAcomplex, aImageCropFftComplex, aInitial);
#else
	wrapped.rawF32Ops.mataUpdate(aMatAcomplex, aImageCropFftComplex, aInitial);
#endif
}

void DebugRawF32Ops::matbUpdate(void *aMatBcomplex, const void *aImageCropFftComplex, bool aInitial)
{
#if 0
	wrapped.opencvNativeRawF32Ops.matbUpdate(aMatBcomplex, aImageCropFftComplex, aInitial);
#elif 1
	wrapped.parallelOps.ops->matbUpdate(aMatBcomplex, aImageCropFftComplex, aInitial);
#else
	wrapped.rawF32Ops.matbUpdate(aMatBcomplex, aImageCropFftComplex, aInitial);
#endif
}

void DebugRawF32Ops::initImpl()
{
#if 1
	wrapped.opencvNativeRawF32Ops.init(roi());
#endif

#if 1
	wrapped.rawF32Ops.init(roi());
#endif

#if 1
	wrapped.parallelOps.ops->init(roi());
#endif
}

}  // namespace Ut
}  // namespace Mosse
