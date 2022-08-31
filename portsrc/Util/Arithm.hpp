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
namespace Impl {

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

template <class T>
struct ConvInv {
	template <bool F>
	using EnableIf = typename std::enable_if<F>::type *;

	template <Tp::Repr::Flags F>
	static inline T convInv(const void *num, EnableIf<(F & Tp::Repr::ReprRaw) && !(F & Tp::Repr::AlignPlatform)>)
	{
		return *reinterpret_cast<const T *>(num);
	}
};

}  // namespace Impl
/// \brief Convert from compact representation to the regular one (decode). E.g. fixed point to regular `float` type
///
template <class T, Tp::Repr::Flags From>
inline T convInv(const void *num)
{
	return Impl::ConvInv<T>::convInv<From>(num, nullptr);
}

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_ARITHM_HPP_
