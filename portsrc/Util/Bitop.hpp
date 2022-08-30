//
// Bitop.hpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_BITOP_HPP_)
#define MOSSE_UTIL_BITOP_HPP_

namespace Mosse {

template <class T>
static constexpr inline std::uint64_t bit(T base)
{
	return 0;
}

template <class B, class T, class ...Ts>
static constexpr inline std::uint64_t bit(B base, T t, Ts ...aTs)
{
	using List = std::uint64_t[];

	return 1 << (t + base) | bit(base, aTs...);
}

template <class B, class N>
static constexpr inline std::uint64_t mask(B base, N n)
{
	return static_cast<int>(n) == 0 ?
		0 :
		0 | bit(base, n - 1) | mask(base, n - 1);
}

}  // namespace Mosse

#endif // MOSSE_UTIL_BITOP_HPP_
