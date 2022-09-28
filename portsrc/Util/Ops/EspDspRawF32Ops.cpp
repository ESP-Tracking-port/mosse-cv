//
// EspDspRawF32Ops.cpp
//
// Created on: Sep 26, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <Fft.h>
#include "Util/Helper/EspDspFft2.hpp"
#include "Port/MossePort.hpp"
#include "EspDspRawF32Ops.hpp"

ohdebuggroup(EspDspRawF32Ops::fft2)
ohdebuggroup(EspDspRawF32Ops::ifft2)

namespace Mosse {
namespace Ut {

EspDspRawF32Ops::EspDspRawF32Ops() : RawF32Ops{}, espDspFft2{}
{
}

void EspDspRawF32Ops::fft2(void *aBufferComplex)
{
	ohdebugsect(EspDspRawF32Ops::fft2, {
		std::size_t bufferLength = roi().area() * 2;
		const float *ptrBuf = static_cast<const float *>(aBufferComplex);

		for (std::size_t i = 0; i < bufferLength; ++i) {
			ohdebug(EspDspRawF32Ops::fft2, i, *(ptrBuf + i), ptrBuf + i);
			assert(!std::isnan(*(ptrBuf + i)));
		}
	});

	espDspFft2.fft2(aBufferComplex);

	ohdebugsect(EspDspRawF32Ops::fft2, {
		std::size_t bufferLength = roi().area() * 2;
		const float *ptrBuf = static_cast<const float *>(aBufferComplex);

		for (std::size_t i = 0; i < bufferLength; ++i) {
			ohdebug(EspDspRawF32Ops::fft2, i, *(ptrBuf + i), ptrBuf + i);
			assert(!std::isnan(*(ptrBuf + i)));
		}
	});
}

void EspDspRawF32Ops::ifft2(void *aBufferComplex)
{
	ohdebugsect(EspDspRawF32Ops::ifft2, {
		for (std::size_t row = roiFragment().origin(0); row < roiFragment().size(0); ++row) {
			for (std::size_t col = roiFragment().origin(1); col < roiFragment().size(1); ++col) {
				assert(!std::isnan(atAsVariant<reprFlags.buffer>({row, col}, roi(), aBufferComplex).f32));
				assert(!std::isnan(atImagAsVariant<reprFlags.buffer>({row, col}, roi(), aBufferComplex).f32));
			}
		}
	});

	espDspFft2.ifft2(aBufferComplex);
}

void EspDspRawF32Ops::initImpl()
{
	RawF32Ops::initImpl();

	if (isFirstInit()) {
		ohdebug(EspDspRawF32Ops::initImpl);
		espDspFft2.init(roi(), nullptr, nullptr);
	}
}

void EspDspRawF32Ops::imageCropInto(Tp::Image aImageReal, void *aBufferComplex)
{
	RawF32Ops::imageCropInto(aImageReal, aBufferComplex);

	ohdebugsect(EspDspRawF32Ops::imageCropInto, {
		for (std::size_t row = roiFragment().origin(0); row < roiFragment().size(0); ++row) {
			for (std::size_t col = roiFragment().origin(1); col < roiFragment().size(1); ++col) {
				assert(!std::isnan(atAsVariant<reprFlags.buffer>({row, col}, roi(), aBufferComplex).f32));
				assert(!std::isnan(atImagAsVariant<reprFlags.buffer>({row, col}, roi(), aBufferComplex).f32));
			}
		}
	});
}

}  // namespace Ut
}  // namespace Mosse
