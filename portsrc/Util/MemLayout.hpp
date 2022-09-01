//
// MemLayout.hpp
//
// Created on: Sep 01, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_MEMLAYOUT_HPP_)
#define MOSSE_UTIL_MEMLAYOUT_HPP_

#include "Types/Repr.hpp"
#include <type_traits>

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

template <Tp::Repr::Flags F>
constexpr unsigned strideInner(typename std::enable_if<F & Tp::Repr::CplxRe1Im1>::type * = nullptr)
{
	return Ut::szof<F>() * 2;
}

template <Tp::Repr::Flags F>
constexpr unsigned strideInner(typename std::enable_if<F & Tp::Repr::CplxRenImn>::type * = nullptr)
{
	return Ut::szof<F>() * 2;
}

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_MEMLAYOUT_HPP_
