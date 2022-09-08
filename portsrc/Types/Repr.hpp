//
// Repr.hpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_TYPES_REPR_HPP_)
#define MOSSE_TYPES_REPR_HPP_

#include "Util/Helper/Bitop.hpp"
#include "Util/Helper/StoreType.hpp"
#include <cstdint>
#include <type_traits>

namespace Mosse {
namespace Tp {

/// \brief The implementation is only expected to work w/ two representations. These flags are defined to leave the
/// room for further painless extension, if the results of the experiments w/ different FFTs and representations are
/// not satisfactory. \see CommonOps for use examples.
///
struct Repr {
	using Flags = std::uint64_t;
	enum : Flags {
		// Storage
		BaseStorage = 0,
		BitsStorage = 2,
		MaskStorage = Ut::mask(BaseStorage, BitsStorage),

		StorageI16 = Ut::bitb(BaseStorage, 0),  ///< Item is packed in 16 bits
		StorageF32 = Ut::bitb(BaseStorage, 1),  ///< Item is packed in 32 bits

		// Repr
		BaseRepr = BaseStorage + BitsStorage,
		BitsRepr = 3,
		MaskRepr = Ut::mask(BaseRepr, BitsRepr),

		ReprRaw = Ut::bitb(BaseRepr, 0),  ///< item = item
		ReprLog2 = Ut::bitb(BaseRepr, 1),  ///< item = static_cast<std::int16_t>(log2(value));
		ReprFixedPoint = Ut::bitb(BaseRepr, 2),  ///< item = static_cast<Integer>(fp * kPrecision);

		// Cplx. If not specified, an array (or an element) is interpreted as real
		BaseCplx = BaseRepr + BitsRepr,
		BitsCplx = 2,
		MaskCplx = Ut::mask(BaseCplx, BitsCplx),

		CplxRe1Im1 = Ut::bitb(BaseCplx, 0),  ///< The array is a complex one. Numbers are placed in (Real1, Im1, Real2, Im2, ...) sequence
		CplxRenImn = Ut::bitb(BaseCplx, 1),  ///< The array is a complex one. Numbers are placed in (Real1, Real2, Im1, Im2, ...) sequence

		// Helper masks
		MaskTraitScalar = MaskStorage | MaskRepr,  ///< Mask that uniquely identifies a scalar
		MaskTraitArray = MaskStorage | MaskRepr | MaskCplx,  ///< Mask that uniquely identifies an array (scalar traits + array layout)
	};

	/// \brief Compile-time type selector
	///
	template <Flags Storage>
	struct TypeImpl;

	/// \brief In which type a value is stored
	///
	template <Flags F>
	using Type = typename TypeImpl<F & MaskStorage>::Type;

	template <class T>
	struct StorageImpl;

	template <class T>
	using Storage = typename StorageImpl<T>::value;

	template <Flags F>
	static constexpr bool isValid()
	{
		return (F & MaskStorage) && (F & MaskRepr);
	}
};

template <>
struct Repr::TypeImpl<Repr::StorageI16> : Ut::StoreType<std::int16_t> {
};

template <>
struct Repr::TypeImpl<Repr::StorageF32> : Ut::StoreType<float> {
};

template <>
struct Repr::StorageImpl<float> : std::integral_constant<Tp::Repr::Flags, Tp::Repr::StorageF32> {
};

template <>
struct Repr::StorageImpl<std::int16_t> : std::integral_constant<Tp::Repr::Flags, Tp::Repr::StorageI16> {
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
