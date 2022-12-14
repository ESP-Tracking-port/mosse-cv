//
// RawF32Ops.cpp
//
// Created on: Sep 05, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <Fft.h>
#include "Mosse.hpp"
#include "MosseApi.hpp"
#include "embmosse/Util/Arithm/MemLayout.hpp"
#include "embmosse/Port/MossePort.hpp"
#include "RawF32Ops.hpp"

static constexpr auto kEta = 0.125f;
static constexpr auto kGetGauss = Mosse::getGaussKernelFft3dScaled125;
static constexpr auto kGetHann = Mosse::getHann;

namespace Mosse {
namespace Ut {

static_assert(kRawF32ReprBuffer & Tp::Repr::CplxRe1Im1, "");  // fft depends on that condition

RawF32Ops::RawF32Ops() : pmHelper{kGetGauss, kGetHann}, fft{}
{
	setEta({kEta}, {1.0f - kEta});
}

void RawF32Ops::fft2(void *aBufferComplex)
{
	// Fft row-wise
	fft.init2(roi().rows(), roi().cols(), Ut::strideInner<kRawF32ReprBuffer>());
	float *re = static_cast<float *>(aBufferComplex) + Ut::offsetFirstReal<kRawF32ReprBuffer>(roi());
	float *im = static_cast<float *>(aBufferComplex) + Ut::offsetFirstImag<kRawF32ReprBuffer>(roi());
	fft.transformDirect2(re, im);

	return;
}

void RawF32Ops::ifft2(void *aBufferComplex)
{
	fft.init2(roi().rows(), roi().cols(), Ut::strideInner<kRawF32ReprBuffer>());
	float *re = static_cast<float *>(aBufferComplex) + Ut::offsetFirstReal<kRawF32ReprBuffer>(roi());
	float *im = static_cast<float *>(aBufferComplex) + Ut::offsetFirstImag<kRawF32ReprBuffer>(roi());
	fft.transformComplement2(re, im);

	return;

}

void RawF32Ops::initImpl()
{
	CommonOps::initImpl();
	pmHelper.update(roi());
}

const void *RawF32Ops::hannMatrix()
{
	return pmHelper.hann();
}

const void *RawF32Ops::gaussFft()
{
	return pmHelper.gauss();
}

#if MOSSE_USE_OPENCV
void RawF32Ops::fft2Common(void *aBufferComplex, bool aFwd)
{
	static_assert(kRawF32ReprBuffer & Tp::Repr::CplxRe1Im1, "");

	const int sizes[2] = {roi().size(0), roi().size(1)};
	auto reim = cv::Mat(2, sizes, CV_32FC2, aBufferComplex);
	cv::dft(reim, reim, !aFwd ? (cv::DFT_SCALE | cv::DFT_INVERSE) : 0);
}
#else
void RawF32Ops::fft2Common(void *, bool)
{
}
#endif

}  // namespace Ut
}  // namespace Mosse
