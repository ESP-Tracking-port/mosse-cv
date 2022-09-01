//
// MemLayout.hpp
//
// Created on: Sep 01, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_MEMLAYOUT_HPP_)
#define MOSSE_UTIL_MEMLAYOUT_HPP_

#include "Types/Repr.hpp"
#include "Types/Tracking.hpp"
#include "Util/Helper/En.h"
#include <type_traits>

namespace Mosse {
namespace Ut {

static std::size_t szof(Tp::Repr::Flags f)
{
	return f & Tp::Repr::Len16 ?
		16 : f & Tp::Repr::Len32 ?
		32 : 0;
}

/// \brief Size of the integral type that is used to store a number in a chosen representation
///
template <Tp::Repr::Flags F>
static constexpr std::size_t szof()
{
	return sizeof(typename Tp::Repr::template Type<F>);
}

/// \brief Stride between two consecutive real or imaginary numbers in a chosen layout
///
template <Tp::Repr::Flags F>
constexpr unsigned strideInner(En<F & Tp::Repr::CplxRe1Im1> = nullptr)
{
	return Ut::szof<F>() * 2;
}

template <Tp::Repr::Flags F>
constexpr unsigned strideInner(En<F & Tp::Repr::CplxRenImn> = nullptr)
{
	return Ut::szof<F>();
}

template <Tp::Repr::Flags F, class T>
constexpr unsigned offsetFirstReal(T &&)
{
	return 0;
}

template <Tp::Repr::Flags F, class T>
constexpr unsigned offsetFirstImag(T &&, En<F & Tp::Repr::CplxRe1Im1> = nullptr)
{
	return Ut::szof<F>();
}

template <Tp::Repr::Flags F>
unsigned offsetFirstImag(Tp::Roi roi, En<F & Tp::Repr::CplxRenImn> = nullptr)
{
	return Ut::szof<F>() * roi.size.rows() * roi.size.cols();
}

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_MEMLAYOUT_HPP_
