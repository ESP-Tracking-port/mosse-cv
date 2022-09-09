//
// DebugRawF32Ops.cpp
//
// Created on: Sep 09, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "OpencvNativeRawF32Ops.hpp"
#include "RawF32Ops.hpp"
#include "DebugRawF32Ops.hpp"

namespace Mosse {
namespace Ut {

DebugRawF32Ops::DebugRawF32Ops() : wrapped{{}, {}}
{
}

void DebugRawF32Ops::imageCropInto(Tp::Image aImageReal, void *aBufferComplex)
{
#if 1
	wrapped.opencvNativeRawF32Ops.imageCropInto(aImageReal, aBufferComplex);
#else
	wrapped.rawF32Ops.imageCropInto(aImageReal, aBufferComplex);
#endif
}

void DebugRawF32Ops::imagePreprocess(void *aCropComplex)
{
#if 1
	wrapped.opencvNativeRawF32Ops.imagePreprocess(aCropComplex);
#else
	wrapped.rawF32Ops.imagePreprocess(aCropComplex);
#endif
}

void DebugRawF32Ops::imageConvFftDomain(void *aioCropFft2Complex, void *aMatrixAcomlex, void *aMatrixBcomplex)
{
#if 1
	wrapped.opencvNativeRawF32Ops.imageConvFftDomain(aioCropFft2Complex, aMatrixAcomlex, aMatrixBcomplex);
#else
	wrapped.rawF32Ops.imageConvFftDomain(aioCropFft2Complex, aMatrixAcomlex, aMatrixBcomplex);
#endif
}

void DebugRawF32Ops::fft2(void *aBufferComplex)
{
#if 1
	wrapped.opencvNativeRawF32Ops.fft2(aBufferComplex);
#else
	wrapped.rawF32Ops.fft2(aBufferComplex);
#endif
}

void DebugRawF32Ops::ifft2(void *aBufferComplex)
{
#if 1
	wrapped.opencvNativeRawF32Ops.ifft2(aBufferComplex);
#else
	wrapped.rawF32Ops.ifft2(aBufferComplex);
#endif
}

void DebugRawF32Ops::maxReal(const void *aBufferComplex, Tp::PointRowCol &aPeakPos, float *sum)
{
#if 1
	wrapped.opencvNativeRawF32Ops.maxReal(aBufferComplex, aPeakPos, sum);
#else
	wrapped.rawF32Ops.maxReal(aBufferComplex, aPeakPos, sum);
#endif
}

float DebugRawF32Ops::calcPsr(const void *aBufferComplex, const Tp::PointRowCol &aPeak, float sumHint, Tp::PointRowCol aMask)
{
#if 1
	wrapped.opencvNativeRawF32Ops.calcPsr(aBufferComplex, aPeak, sumHint, aMask);
#else
	wrapped.rawF32Ops.calcPsr(aBufferComplex, aPeak, sumHint, aMask);
#endif
}

void DebugRawF32Ops::mataUpdate(void *aMatAcomplex, const void *aImageCropFftComplex, bool aInitial)
{
#if 1
	wrapped.opencvNativeRawF32Ops.mataUpdate(aMatAcomplex, aImageCropFftComplex, aInitial);
#else
	wrapped.rawF32Ops.mataUpdate(aMatAcomplex, aImageCropFftComplex, aInitial);
#endif
}

void DebugRawF32Ops::matbUpdate(void *aMatBcomplex, const void *aImageCropFftComplex, bool aInitial)
{
#if 1
	wrapped.opencvNativeRawF32Ops.matbUpdate(aMatBcomplex, aImageCropFftComplex, aInitial);
#else
	wrapped.rawF32Ops.matbUpdate(aMatBcomplex, aImageCropFftComplex, aInitial);
#endif
}

void DebugRawF32Ops::initImpl()
{
#if 1
	wrapped.opencvNativeRawF32Ops.initImpl();
#else
	wrapped.rawF32Ops.initImpl();
#endif
}

const void *DebugRawF32Ops::hannMatrix()
{
#if 1
	return wrapped.opencvNativeRawF32Ops.hannMatrix();
#else
	return wrapped.rawF32Ops.hannMatrix();
#endif
}

const void *DebugRawF32Ops::gaussFft()
{
#if 1
	return wrapped.opencvNativeRawF32Ops.gaussFft();
#else
	return wrapped.rawF32Ops.gaussFft();
#endif
}

}  // namespace Ut
}  // namespace Mosse
