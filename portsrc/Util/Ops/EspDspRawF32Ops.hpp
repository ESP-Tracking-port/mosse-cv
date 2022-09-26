//
// EspDspRawF32Ops.hpp
//
// Created on: Sep 26, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef ESPDSPRAWF32OPS_HPP
#define ESPDSPRAWF32OPS_HPP

#include "Util/Ops/RawF32Ops.hpp"
#include <memory>

namespace Mosse {
namespace Ut {

class EspDspFft2Base;

class EspDspRawF32Ops : public RawF32Ops {
public:
	EspDspRawF32Ops();
	void fft2(void *aBufferComplex) override;
	void ifft2(void *aBufferComplex) override;
private:
	std::unique_ptr<EspDspFft2Base> espDspFft2;
};

}  // namespace Ut
}  // namespace Mosse

#endif // ESPDSPRAWF32OPS_HPP
