//
// Arithm.hpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_ARITHM_HPP_)
#define MOSSE_UTIL_ARITHM_HPP_

#include "Types/Repr.hpp"

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

template <class T>
struct FromRepr {
	template <bool F>
	using EnableIf = typename std::enable_if<F>::type *;

	template <Tp::Repr::Flags F>
	static inline T call(const void *num, EnableIf<(F & Tp::Repr::ReprRaw) && !(F & Tp::Repr::AlignPlatform)>)
	{
		return static_cast<T>(*static_cast<const typename Tp::Repr::template Type<F> *>(num));
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
