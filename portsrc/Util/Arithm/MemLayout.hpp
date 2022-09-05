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

inline std::size_t szof(Tp::Repr::Flags f)
{
	return f & Tp::Repr::StorageI16 ?
		16 : f & Tp::Repr::StorageF32 ?
		32 : 0;
}

/// \brief Size of the integral type that is used to store a number in a chosen representation
///
template <Tp::Repr::Flags F>
inline constexpr std::size_t szof()
{
	return sizeof(typename Tp::Repr::template Type<F>);
}

// Check that overrides provided for various array layouts are comprehensive

static_assert(countBit(Tp::Repr::MaskCplx) == 2, "It is expected that there are only 2 array layouts available."
	"If that is not the case, the implementation better be double checked");

/// \brief Stride between two consecutive real or imaginary numbers in a chosen layout. Override for ReImReIm layout.
///
template <Tp::Repr::Flags F>
constexpr unsigned strideInner(En<F & Tp::Repr::CplxRe1Im1> = nullptr)
{
	return Ut::szof<F>() * 2;
}

/// \brief Override for ReReRe...ImImIm... layout
template <Tp::Repr::Flags F>
constexpr unsigned strideInner(En<F & Tp::Repr::CplxRenImn> = nullptr)
{
	return Ut::szof<F>();
}

/// \brief Override for non-complex array (ReRe...Re layout)
template <Tp::Repr::Flags F>
constexpr unsigned strideInner(En<!(F & Tp::Repr::MaskCplx)> = nullptr)
{
	return Ut::szof<F>();
}

template <Tp::Repr::Flags F, class R = std::uint8_t>
constexpr unsigned offsetFirstReal(const Tp::Roi &)
{
	return 0 * sizeof(R);
}

template <Tp::Repr::Flags F, class R = std::uint8_t>
constexpr unsigned offsetFirstImag(const Tp::Roi &, En<F & Tp::Repr::CplxRe1Im1> = nullptr)
{
	return Ut::szof<F>() * sizeof(R);
}

template <Tp::Repr::Flags F, class R = std::uint8_t>
inline unsigned offsetFirstImag(const Tp::Roi &roi, En<F & Tp::Repr::CplxRenImn> = nullptr)
{
	return Ut::szof<F>() * roi.size.rows() * roi.size.cols() * sizeof(R);
}

template <Tp::Repr::Flags F>
inline void *at(unsigned offset, void *mem)
{
	using ValueType = typename Tp::Repr::Type<F>;
	return static_cast<void *>(static_cast<ValueType *>(mem) + offset);
}

template <Tp::Repr::Flags F>
inline void *at(unsigned offset, const void *mem)
{
	using ValueType = typename Tp::Repr::Type<F>;
	return static_cast<const void *>(static_cast<const ValueType *>(mem) + offset);
}

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_MEMLAYOUT_HPP_
