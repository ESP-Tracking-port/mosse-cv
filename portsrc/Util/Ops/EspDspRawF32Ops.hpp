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

template <Tp::Repr::Flags>
class EspDspFft2;

class EspDspRawF32Ops : public RawF32Ops {
public:
	EspDspRawF32Ops();
	void fft2(void *aBufferComplex) override;
	void ifft2(void *aBufferComplex) override;
	void initImpl() override;
	void imageCropInto(Tp::Image aImageReal, void *aBufferComplex) override;
private:
	EspDspFft2<RawF32Ops::reprFlags.buffer> espDspFft2;
};

}  // namespace Ut
}  // namespace Mosse

#endif // ESPDSPRAWF32OPS_HPP
