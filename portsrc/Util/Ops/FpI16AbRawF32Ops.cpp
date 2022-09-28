//
// RawF32Ops.cpp
//
// Created on: Sep 05, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <Fft.h>
#include "Mosse.hpp"
#include "MosseApi.hpp"
#include "Util/Arithm/MemLayout.hpp"
#include "Port/MossePort.hpp"

static constexpr auto kEta = 0.125f;
static constexpr auto kGetGauss = Mosse::getGaussKernelFft3dScaled125;
static constexpr auto kGetHann = Mosse::getHann;

namespace Mosse {
namespace Ut {

static_assert(kFpI16AbRawF32ReprBuffer & Tp::Repr::CplxRe1Im1, "");  // fft depends on that condition

FpI16AbRawF32Ops::FpI16AbRawF32Ops() : pmHelper{kGetGauss, kGetHann}, fft{}
{
	setEta({kEta}, {1.0f - kEta});
}

void FpI16AbRawF32Ops::fft2(void *aBufferComplex)
{
	// Fft row-wise
	fft.init2(roi().rows(), roi().cols(), Ut::strideInner<kRawF32ReprBuffer>());
	float *re = static_cast<float *>(aBufferComplex) + Ut::offsetFirstReal<kRawF32ReprBuffer>(roi());
	float *im = static_cast<float *>(aBufferComplex) + Ut::offsetFirstImag<kRawF32ReprBuffer>(roi());
	fft.transformDirect2(re, im);

	return;
}

void FpI16AbRawF32Ops::ifft2(void *aBufferComplex)
{
	fft.init2(roi().rows(), roi().cols(), Ut::strideInner<kRawF32ReprBuffer>());
	float *re = static_cast<float *>(aBufferComplex) + Ut::offsetFirstReal<kRawF32ReprBuffer>(roi());
	float *im = static_cast<float *>(aBufferComplex) + Ut::offsetFirstImag<kRawF32ReprBuffer>(roi());
	fft.transformComplement2(re, im);

	return;

}

void FpI16AbRawF32Ops::initImpl()
{
	CommonOps::initImpl();
	pmHelper.update(roi());
}

const void *FpI16AbRawF32Ops::hannMatrix()
{
	return pmHelper.hann();
}

const void *FpI16AbRawF32Ops::gaussFft()
{
	return pmHelper.gauss();
}

}  // namespace Ut
}  // namespace Mosse
