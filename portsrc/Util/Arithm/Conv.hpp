//
// Conv.hpp
//
// Created on: Sep 01, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_ARITHM_CONV_HPP_)
#define MOSSE_UTIL_ARITHM_CONV_HPP_

#include "Types/Repr.hpp"
#include <cmath>

namespace Mosse {
namespace Ut {
namespace Impl {

template <bool F>
using En = typename std::enable_if<F>::type *;

template <class T>
struct FromRepr;

template <>
struct FromRepr<float> {

	template <Tp::Repr::Flags F>
	static inline float call(const float &a, En<F & Tp::Repr::ReprRaw>)
	{
		return a;
	}

	template <Tp::Repr::Flags F>
	static inline float call(const std::int16_t &a, En<F & Tp::Repr::ReprLog2>)
	{
		return pow(2.0f, a);
	}
};

}  // namespace Impl

/// \brief Convert from compact representation to the regular one (decode). E.g. fixed point to regular `float` type
///
template <class T, Tp::Repr::Flags From>
inline T fromRepr(const void *num)
{
	return Impl::FromRepr<T>::template call<From>(num, nullptr);
}

namespace Impl {

template <class T>
struct ToRepr;

template <>
struct ToRepr<std::uint8_t> {
	template <Tp::Repr::Flags F>
	inline static auto call(...) -> typename Tp::Repr::template Type<F>
	{
		return 0;
	}
};

}  // namespace Impl

template <class T, Tp::Repr::Flags To>
inline auto toRepr(std::uint8_t a) -> typename Tp::Repr::template Type<To>
{
}

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_ARITHM_CONV_HPP_
