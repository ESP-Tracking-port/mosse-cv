//
// RawF32Ops.cpp
//
// Created on: Sep 05, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Mosse.hpp"
#include "MosseApi.hpp"
#include "RawF32Ops.hpp"
#include "Util/Arithm/MemLayout.hpp"
#include "MossePort.hpp"
#if MOSSE_USE_OPENCV
#endif  // MOSSE_USE_OPENCV

static constexpr auto kEta = 0.125f;
static constexpr auto kGetGauss = Mosse::getGaussKernelFft3dScaled125;
static constexpr auto kGetHann = Mosse::getHann;

namespace Mosse {
namespace Ut {

static_assert(kRawF32ReprBuffer & Tp::Repr::CplxRe1Im1, "");  // fft depends on that condition

RawF32Ops::RawF32Ops() : pmHelper{kGetGauss, kGetHann}
{
	setEta(kEta);
}

void RawF32Ops::fft2(void *aBufferComplex)
{
	fft2Common(aBufferComplex, true);
}

void RawF32Ops::ifft2(void *aBufferComplex)
{
	fft2Common(aBufferComplex, false);
}

void RawF32Ops::initImpl()
{
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

	ohdebugsecteveryn(-1, {
		ohdebug(RawF32Ops::fft2common, aFwd, "before", reim);
	});

	cv::dft(reim, reim, !aFwd ? (cv::DFT_SCALE | cv::DFT_INVERSE) : 0);

	ohdebugsecteveryn(-1, {
		ohdebug(RawF32Ops::fft2common, aFwd, "after", reim);
	});
}
#else
void RawF32Ops::fft2Common(void *, bool)
{
}
#endif

}  // namespace Ut
}  // namespace Mosse
