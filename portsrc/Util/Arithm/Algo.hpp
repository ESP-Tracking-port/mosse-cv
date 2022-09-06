//
// Algo.hpp
//
// Created on: Sep 06, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_ARITHM_ALGO_HPP_)
#define MOSSE_UTIL_ARITHM_ALGO_HPP_

#include <limits>

namespace Mosse {
namespace Ut {

template <class T>
void clamp(T &val, T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max())
{
	val = val < min ? min :
		val > max ? max :
		val;
}

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_ARITHM_ALGO_HPP_
