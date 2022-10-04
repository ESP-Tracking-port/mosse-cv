//
// ReTp.hpp
//
// Created on: Sep 02, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_HELPER_RETP_HPP_)
#define MOSSE_UTIL_HELPER_RETP_HPP_

#include "embmosse/Types/Repr.hpp"

namespace Mosse {
namespace Ut {

/// \brief Shortcut type getter for Ut namespace. ReTp stands for "REpresentation TyPe"
///
template <Tp::Repr::Flags F>
using ReTp = typename Tp::Repr::Type<F>;

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_HELPER_RETP_HPP_
