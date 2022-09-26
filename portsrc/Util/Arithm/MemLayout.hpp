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
#include "Util/Helper/ReTp.hpp"
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

/// \brief Pointer increment between two consecutive real or imaginary numbers in a chosen layout. Override for
/// ReImReIm layout.
///
template <Tp::Repr::Flags F>
constexpr unsigned strideInner(En<F & Tp::Repr::CplxRe1Im1> = nullptr)
{
	return 2;
}

/// \brief Override for ReReRe...ImImIm... layout
///
template <Tp::Repr::Flags F>
constexpr unsigned strideInner(En<F & Tp::Repr::CplxRenImn> = nullptr)
{
	return 1;
}

/// \brief Override for non-complex array (ReRe...Re layout)
///
template <Tp::Repr::Flags F>
constexpr unsigned strideInner(En<!(F & Tp::Repr::MaskCplx)> = nullptr)
{
	return 1;
}

/// \brief Pointer increment between two elements in one column
///
template <Tp::Repr::Flags F>
constexpr unsigned strideOuter(const Tp::Roi &roi)
{
	return strideInner<F>() * roi.cols();
}

template <Tp::Repr::Flags F>
constexpr unsigned offsetFirstReal(const Tp::Roi &)
{
	return 0;
}

template <Tp::Repr::Flags F, class R = std::uint8_t>
constexpr unsigned offsetFirstImag(const Tp::Roi &, En<F & Tp::Repr::CplxRe1Im1> = nullptr)
{
	return 1;
}

template <Tp::Repr::Flags F, class R = std::uint8_t>
inline unsigned offsetFirstImag(const Tp::Roi &roi, En<F & Tp::Repr::CplxRenImn> = nullptr)
{
	return roi.rows() * roi.cols();
}

/// \brief Maps coordinates of a point onto array
///
template <Tp::Repr::Flags F>
inline unsigned pointOffset(const Tp::PointRowCol &point, const Tp::Roi &roi)
{
	return strideOuter<F>(roi) * point(0) + point(1) * strideInner<F>();
}

template <Tp::Repr::Flags F>
inline void *at(unsigned offset, void *mem)
{
	using ValueType = typename Tp::Repr::Type<F>;
	return static_cast<void *>(static_cast<ValueType *>(mem) + offset);
}

template <Tp::Repr::Flags F>
inline const void *at(unsigned offset, const void *mem)
{
	using ValueType = typename Tp::Repr::Type<F>;
	return static_cast<const void *>(static_cast<const ValueType *>(mem) + offset);
}

template <Tp::Repr::Flags F, class B>
inline auto at(const Tp::PointRowCol &point, const Tp::Roi &roi, B mem) -> B
{
	return at<F>(pointOffset<F>(point, roi), mem);
}

template <Tp::Repr::Flags F, class B>
inline auto atImag(const Tp::PointRowCol &point, const Tp::Roi &roi, B mem) -> B
{
	return at<F>(offsetFirstImag<F>(roi) + pointOffset<F>(point, roi), mem);
}

template <Tp::Repr::Flags F, class ...Ts>
inline Tp::NumVariant atAsVariant(Ts &&...aArgs)
{
	return {*static_cast<const ReTp<F> *>(at<F>(std::forward<Ts>(aArgs)...))};
}

template <Tp::Repr::Flags F, class B>
inline Tp::NumVariant atImagAsVariant(const Tp::PointRowCol &point, const Tp::Roi &roi, B mem)
{
	return {*static_cast<const ReTp<F> *>(atImag<F>(point, roi, mem))};
}

struct MemLayoutBase {
	virtual Tp::NumVariant atAsVariant(const Tp::PointRowCol &aCoordinates, const Tp::Roi &aRoi, void *mem) const = 0;
	virtual Tp::NumVariant atAsVariant(const Tp::PointRowCol &aCoordinates, const Tp::Roi &aRoi,
		const void *mem) const = 0;
};

template <Tp::Repr::Flags F>
struct MemLayout : MemLayoutBase {
	Tp::NumVariant atAsVariant(const Tp::PointRowCol &aCoordinates, const Tp::Roi &aRoi, void *mem) const override
	{
		return Ut::atAsVariant<F>(aCoordinates, aRoi, mem);
	}

	Tp::NumVariant atAsVariant(const Tp::PointRowCol &aCoordinates, const Tp::Roi &aRoi, const void *mem) const override
	{
		return Ut::atAsVariant<F>(aCoordinates, aRoi, mem);
	}
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_MEMLAYOUT_HPP_
