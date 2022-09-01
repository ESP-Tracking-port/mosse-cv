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

template <Tp::Repr::Flags F, class R = std::uint8_t>
constexpr unsigned offsetFirstReal(Tp::Roi)
{
	return 0 * sizeof(R);
}

template <Tp::Repr::Flags F, class R = std::uint8_t>
constexpr unsigned offsetFirstImag(Tp::Roi, En<F & Tp::Repr::CplxRe1Im1> = nullptr)
{
	return Ut::szof<F>() * sizeof(R);
}

template <Tp::Repr::Flags F, class R = std::uint8_t>
unsigned offsetFirstImag(Tp::Roi roi, En<F & Tp::Repr::CplxRenImn> = nullptr)
{
	return Ut::szof<F>() * roi.size.rows() * roi.size.cols() * sizeof(R);
}

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_MEMLAYOUT_HPP_
