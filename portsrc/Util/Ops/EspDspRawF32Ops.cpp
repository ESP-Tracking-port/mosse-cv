//
// EspDspRawF32Ops.cpp
//
// Created on: Sep 26, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <Fft.h>
#include "Util/Helper/EspDspFft2.hpp"
#include "EspDspRawF32Ops.hpp"

namespace Mosse {
namespace Ut {

EspDspRawF32Ops::EspDspRawF32Ops() : RawF32Ops{}, espDspFft2{new EspDspFft2<reprFlags.buffer>{roi(), nullptr, nullptr}}
{
}

void EspDspRawF32Ops::fft2(void *aBufferComplex)
{
	espDspFft2->fft2(aBufferComplex);
}

void EspDspRawF32Ops::ifft2(void *aBufferComplex)
{
	espDspFft2->ifft2(aBufferComplex);
}

}  // namespace Ut
}  // namespace Mosse
