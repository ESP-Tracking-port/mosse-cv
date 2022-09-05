//
// RawF32Ops.hpp
//
// Created on: Sep 05, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_OPS_RAWF32OPS_HPP_)
#define MOSSE_OPS_RAWF32OPS_HPP_

#include "Util/Ops/CommonOps.hpp"

namespace Mosse {
namespace Ut {

constexpr auto kRawF32ReprBuffer = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw | Tp::Repr::CplxRe1Im1;
constexpr auto kRawF32ReprHannMatrix = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw;
constexpr auto kRawF32ReprAb = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw | Tp::Repr::CplxRe1Im1;
constexpr auto kRawF32ReprAbDivIntermediary = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw;
constexpr auto kRawF32ReprGauss = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw | Tp::Repr::CplxRe1Im1;

using RawF32OpsBase = CommonOps< kRawF32ReprBuffer, kRawF32ReprHannMatrix, kRawF32ReprAb, kRawF32ReprAbDivIntermediary,
	kRawF32ReprGauss>;

/// \brief A naive implementation designed to test the implementation, and to serve as a starting point for
/// implementing other memory-optimized variants of replaceable Ops instances.
///
class RawF32Ops : public RawF32OpsBase {
public:
	virtual void fft2(void *aBufferComplex) = 0;
	virtual void ifft2(void *aBufferComplex) = 0;
protected:
	virtual void initImpl();
	virtual const void *hannMatrix();  ///< Precompiled hann matrix
	virtual const void *gaussFft();  ///< Fouried-transformed precompiled Gaussian matrix
private:
	Tp::Roi roiPrev;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_OPS_RAWF32OPS_HPP_
