//
// Conv.hpp
//
// Created on: Sep 01, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_ARITHM_CONV_HPP_)
#define MOSSE_UTIL_ARITHM_CONV_HPP_

#include "Types/Repr.hpp"
#include "Util/Helper/ReTp.hpp"
#include <cmath>
#include <type_traits>

namespace Mosse {
namespace Ut {
namespace Impl {

template <bool F>
using En = typename std::enable_if<F>::type *;

template <class T>
struct FromRepr;

template <>
struct FromRepr<float> {

	template <Tp::Repr::Flags F, class T>
	static inline float call(const T &repr)
	{
		return static_cast<float>(repr);
	}

	template <Tp::Repr::Flags F>
	static inline float call(std::int16_t a, En<F & Tp::Repr::ReprLog2> = nullptr)
	{
		return pow(2.0f, a);
	}
};

}  // namespace Impl

/// \brief Convert from compact representation to the regular one (decode). E.g. fixed point to regular `float` type
///
template <class T, Tp::Repr::Flags From>
inline T fromRepr(const ReTp<From> &num)
{
	return Impl::FromRepr<T>::template call<From & Tp::Repr::MaskTraitScalar>(num);
}

namespace Impl {

template <class T>
struct ToRepr {
	template <Tp::Repr::Flags F>
	static inline ReTp<F> call(T a, En<F & (Tp::Repr::ReprRaw)> = nullptr)
	{
		return static_cast<ReTp<F>>(a);
	}
};

}  // namespace Impl

template <Tp::Repr::Flags To, class T>
inline ReTp<To> toRepr(T a)
{
	return Impl::template ToRepr<typename std::remove_reference<T>::type>::template call<To>(a);
}

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_ARITHM_CONV_HPP_
