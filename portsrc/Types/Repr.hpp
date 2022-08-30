//
// Repr.hpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_TYPES_REPR_HPP_)
#define MOSSE_TYPES_REPR_HPP_

#include <cstdint>
#include "Util/Bitop.hpp"

namespace Mosse {
namespace Tp {

struct Repr {
	using Flags = std::uint64_t;
	enum : Flags {
		// Len
		BaseLen = 0,
		BitsLen = 2,
		MaskLen = Ut::mask(BaseLen, BitsLen),

		Len16 = Ut::bit(BaseLen, 0),  ///< Item is packed in 16 bits
		Len32 = Ut::bit(BaseLen, 1),  ///< Item is packed in 32 bits

		// Repr
		BaseRepr = BaseLen + BitsLen,
		BitsRepr = 2,
		MaskRepr = Ut::mask(BaseRepr, BitsRepr),

		ReprDirect = Ut::bit(BaseRepr, 0),  ///< item = item
		ReprLog2 = Ut::bit(BaseRepr, 1),  ///< item = static_cast<std::int16_t>(log2(value));

		// Cplx
		BaseCplx = BaseRepr + BitsRepr,
		BitsCplx = 3,
		MaskCplx = Ut::mask(BaseCplx, BitsCplx),

		CplxNone = Ut::bit(BaseCplx, 0),  ///< The array is an array of real numbers.
		CplxIrir = Ut::bit(BaseCplx, 1),  ///< The array is a complex one. Numbers are placed in (Real1, Im1, Real2, Im2, ...) sequence
		CplxIirr = Ut::bit(BaseCplx, 2),  ///< The array is a complex one. Numbers are placed in (Real1, Real2, Im1, Im2, ...) sequence
	};

	template <Flags>
	struct TypeImpl;

	template <Flags F>
	using Type = typename TypeImpl<F | MaskLen>::Type;
};

template <>
struct Repr::TypeImpl<Repr::Len16> {
	using Type = std::int16_t;
};

template <>
struct Repr::TypeImpl<Repr::Len32> {
	using Type = float;
};

class Geometry {
	Repr::Flags buffer;
	Repr::Flags matA;
	Repr::Flags matB;
	Repr::Flags gaussFft;
	Repr::Flags hannMatrix;
};

}  // namespace Tp
}  // namespace Mosse

#endif  // MOSSE_TYPES_REPR_HPP_