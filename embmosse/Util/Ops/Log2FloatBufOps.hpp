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

constexpr auto kLog2FboReprBuffer = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw | Tp::Repr::CplxRe1Im1;
constexpr auto kLog2FboReprHannMatrix = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw;
constexpr auto kLog2FboReprAb = Tp::Repr::StorageI16 | Tp::Repr::ReprLog2;
constexpr auto kLog2FboReprAbDivIntermediary = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw;
constexpr auto kLog2FboReprGauss = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw;
constexpr auto kLog2FboReprEta = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw;

using Log2FloatBufOpsBase = CommonOps<kLog2FboReprBuffer,
	kLog2FboReprHannMatrix,
	kLog2FboReprAb,
	kLog2FboReprAbDivIntermediary,
	kLog2FboReprGauss,
	kLog2FboReprEta>;

class Log2FloatBufOps : Log2FloatBufOpsBase {
public:
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_OPS_LOG2FLOATBUFOPS_HPP_
