//
// Fft.hpp
//
// Created on: Aug 29, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_FFT_HPP_)
#define MOSSE_UTIL_FFT_HPP_

namespace Mosse {
namespace Tp {

class Roi;

}  // namespace Tp

namespace Ut {

class Fft {
public:
	virtual void init(Tp::Roi) = 0;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_FFT_HPP_
