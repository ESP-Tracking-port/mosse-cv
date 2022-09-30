//
// Bitop.hpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_BITOP_HPP_)
#define MOSSE_UTIL_BITOP_HPP_

#include <cstdint>

namespace Mosse {
namespace Ut {

using BitBase = std::uint64_t;

constexpr inline BitBase bit()
{
	return 0;
}

template <class ...Ts>
constexpr BitBase bit(BitBase i, Ts ...ts)
{
	return (1 << i) | bit(ts...);
}

template <class ...Ts>
constexpr BitBase bitb(BitBase base, Ts ...ts)
{
	return bit(ts...) << base;
}

template <class B, class N>
constexpr inline BitBase mask(B base, N n)
{
	return static_cast<int>(n) == 0 || base >= sizeof(BitBase) ?
		0 :
		0 | bitb(base, n - 1) | mask(base, n - 1);
}

constexpr BitBase countBit(BitBase mask, BitBase offset = 0, BitBase count = 0)
{
	return offset >= sizeof(mask) ?
		count :
		mask & bit(offset) ?
		countBit(mask, offset + 1, count + 1) :
		countBit(mask, offset + 1, count);
}

/// \brief Produces ABAB mask pattern taking storing and masked types' sizeofs into account.
/// E.g., if P is std::uint64_t, and N == 8, the result will be 0x00FF00FF00FF00FF
///
template <class P, BitBase Pattern>
constexpr inline P maskAb(BitBase base = 0)
{
	return base > sizeof(P) ?
		0 :
		0 | mask(base, Pattern) | maskAb<P, Pattern>(base + Pattern * 2);
}

constexpr inline BitBase trailingZeros(BitBase bit, BitBase zeros = 0)
{
	return bit == 0 ? sizeof(BitBase) :
		bit & 1 ? zeros :
		trailingZeros(bit >> 1, zeros + 1);
}

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_BITOP_HPP_
