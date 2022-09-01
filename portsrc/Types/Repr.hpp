//
// Repr.hpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_TYPES_REPR_HPP_)
#define MOSSE_TYPES_REPR_HPP_

#include <cstdint>
#include "Util/Helper/Bitop.hpp"
#include "Util/Helper/StoreType.hpp"

namespace Mosse {
namespace Tp {

/// \brief The implementation is only expected to work w/ two representations. These flags are defined to leave the
/// room for further painless extension, if the results of the experiments w/ different FFTs and representations are
/// not satisfactory.
///
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
		BitsRepr = 3,
		MaskRepr = Ut::mask(BaseRepr, BitsRepr),

		ReprRaw = Ut::bit(BaseRepr, 0),  ///< item = item
		ReprLog2 = Ut::bit(BaseRepr, 1),  ///< item = static_cast<std::int16_t>(log2(value));
		ReprFixedPoint = Ut::bit(BaseRepr, 2),  ///< item = static_cast<Integer>(fp * kPrecision);

		// Cplx. If not specified, an array (or an element) is interpreted as real
		BaseCplx = BaseRepr + BitsRepr,
		BitsCplx = 2,
		MaskCplx = Ut::mask(BaseCplx, BitsCplx),

		CplxRe1Im1 = Ut::bit(BaseCplx, 0),  ///< The array is a complex one. Numbers are placed in (Real1, Im1, Real2, Im2, ...) sequence
		CplxRenImn = Ut::bit(BaseCplx, 1),  ///< The array is a complex one. Numbers are placed in (Real1, Real2, Im1, Im2, ...) sequence
	};

	template <Flags Len, Flags Repr>
	struct TypeImpl;

	/// \brief In which type a value is stored
	///
	template <Flags F>
	using Type = typename TypeImpl<F & MaskLen, F & MaskRepr>::Type;

	template <Flags F>
	static constexpr bool isValid()
	{
		return (F & MaskLen) && (F & MaskRepr);
	}
};

template <Repr::Flags F>
struct Repr::TypeImpl<Repr::Len16, F> : Ut::StoreType<std::int16_t> {
};

template <>
struct Repr::TypeImpl<Repr::Len32, Repr::ReprRaw> : Ut::StoreType<float> {
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
