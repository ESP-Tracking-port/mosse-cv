//
// Arithm.hpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_ARITHM_HPP_)
#define MOSSE_UTIL_ARITHM_HPP_

#include "Types/Repr.hpp"
#include <cmath>

namespace Mosse {
namespace Ut {
static std::size_t szof(Tp::Repr::Flags f)
{
	return f & Tp::Repr::Len16 ?
		16 : f & Tp::Repr::Len32 ?
		32 : 0;
}

template <Tp::Repr::Flags F>
static constexpr std::size_t szof()
{
	return sizeof(typename Tp::Repr::template Type<F>);
}

namespace Impl {

template <bool F>
using En = std::enable_if<F>::type *;

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

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_ARITHM_HPP_
