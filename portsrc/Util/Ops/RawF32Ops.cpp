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
# include <opencv2/opencv.hpp>
#endif  // MOSSE_USE_OPENCV

namespace Mosse {
namespace Ut {

static_assert(kRawF32ReprBuffer & Tp::Repr::CplxRe1Im1, "");  // fft depends on that condition

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
	static constexpr auto gaussKernelScaledGet = Mosse::getGaussKernelFft3dScaled125;
	static constexpr auto kEta = 0.125f;
	ohdebug(RawF32OpsBase::initImpl, roiSizePrev);

	if (roiSizePrev != roi().size) {
		roiSizePrev = roi().size;
		precompiledMatrices = {
			Mosse::getHann(roi().size(0), roi().size(1)),
			gaussKernelScaledGet(roi().size(0), roi().size(1))
		};
		setEta(kEta);
		ohdebug(RawF32Ops::initImpl, precompiledMatrices.hann != nullptr);
	}
}

const void *RawF32Ops::hannMatrix()
{
	return precompiledMatrices.hann;
}

const void *RawF32Ops::gaussFft()
{
	return precompiledMatrices.gaussFftScaled;
}

#if MOSSE_USE_OPENCV
void RawF32Ops::fft2Common(void *aBufferComplex, bool aFwd)
{
	static_assert(kRawF32ReprBuffer & Tp::Repr::CplxRe1Im1, "");

	const int sizes[2] = {roi().size(0), roi().size(1)};
	auto reim = cv::Mat(2, sizes, CV_32FC2, aBufferComplex);
	cv::dft(reim, reim, aFwd ? cv::DFT_SCALE : cv::DFT_INVERSE);
}
#else
void RawF32Ops::fft2Common(void *, bool)
{
}
#endif

}  // namespace Ut
}  // namespace Mosse
