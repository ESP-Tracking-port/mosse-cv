//
// FpmHelper.hpp
//
// Created on: Sep 28, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_HELPER_FPMHELPER_HPP_)
#define MOSSE_UTIL_HELPER_FPMHELPER_HPP_

#include "portsrc/Types/Repr.hpp"
#include "portsrc/Util/Helper/StoreType.hpp"
#include "portsrc/Util/Helper/ReTp.hpp"
#include <fpm/fixed.hpp>

namespace Mosse {
namespace Ut {

template <Tp::Repr::Flags F, class T>
inline fpm::fixed<ReTp<F>, std::int32_t, Tp::Repr::FractionBits<F>::value> makeFpmFixed(T aNumber)
{
	return fpm::fixed<ReTp<F>, std::int32_t, Tp::Repr::FractionBits<F>::value>{aNumber};
}

template <Tp::Repr::Flags F, class T>
inline constexpr fpm::fixed<ReTp<F>, std::int32_t, Tp::Repr::FractionBits<F>::value> makeFpmFixedCxe(T aNumber)
{
	return fpm::fixed<ReTp<F>, std::int32_t, Tp::Repr::FractionBits<F>::value>{aNumber};
}

template <Tp::Repr::Flags F>
inline fpm::fixed<ReTp<F>, std::int32_t, Tp::Repr::FractionBits<F>::value> makeFpmFixedFromRaw(ReTp<F> aRaw)
{
	return fpm::fixed<ReTp<F>, std::int32_t, Tp::Repr::FractionBits<F>::value>::from_raw_value(aRaw);
}

/// \brief Returns lowest-possible number for a given representation. Useful for preventing zero division.
///
template <Tp::Repr::Flags F>
inline fpm::fixed<ReTp<F>, std::int32_t, Tp::Repr::FractionBits<F>::value> makeFpmFixedEpsilon()
{
	return std::numeric_limits<fpm::fixed<ReTp<F>, std::int32_t, Tp::Repr::FractionBits<F>::value>>::epsilon();
}

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_HELPER_FPMHELPER_HPP_
