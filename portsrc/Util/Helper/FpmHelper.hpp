//
// FpmHelper.hpp
//
// Created on: Sep 28, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_HELPER_FPMHELPER_HPP_)
#define MOSSE_UTIL_HELPER_FPMHELPER_HPP_

#include "Types/Repr.hpp"
#include "Util/Helper/StoreType.hpp"
#include <fpm/fixed.hpp>

namespace Mosse {
namespace Ut {
namespace Impl {

template <Tp::Repr::Flags F>
struct FpmFixed;

template <>
struct FpmFixed<Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint> : Ut::StoreType<fpm::fixed<std::int16_t,
	std::int32_t, Tp::Repr::FractionBits<Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint>::value>
{
};

}  // namespace Impl

template <Tp::Repr::Flags F>
using FpmFixed = typename Impl::FpmFixed<F & Tp::Repr::MaskTraitScalar>::Type;

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_HELPER_FPMHELPER_HPP_
