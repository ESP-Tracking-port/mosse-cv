//
// Log2FloatBufOps.hpp
//
// Created on: Aug 31, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_OPS_LOG2FLOATBUFOPS_HPP_)
#define MOSSE_UTIL_OPS_LOG2FLOATBUFOPS_HPP_

#include "CommonOps.hpp"

namespace Mosse {
namespace Ut {

static constexpr auto kReprBuffer = Tp::Repr::Len32 | Tp::Repr::ReprRaw | Tp::Repr::CplxRe1Im1;
static constexpr auto kReprHannMatrix = Tp::Repr::Len32 | Tp::Repr::ReprRaw;

class Log2FloatBufOps : CommonOps<kReprBuffer, kReprHannMatrix> {
public:
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_OPS_LOG2FLOATBUFOPS_HPP_
