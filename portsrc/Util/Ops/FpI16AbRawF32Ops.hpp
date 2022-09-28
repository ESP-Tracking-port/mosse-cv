//
// FpI16AbRawF32Ops.hpp
//
// Created on: Sep 05, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_OPS_FPI16ABRAWF32OPS_HPP_)
#define MOSSE_UTIL_OPS_FPI16ABRAWF32OPS_HPP_

#include "Util/Helper/PrecompiledMatrixHelper.hpp"
#include "Util/Ops/CommonOps.hpp"

class Fft;

namespace Mosse {
namespace Ut {

constexpr auto kFpI16AbRawF32ReprBuffer = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw | Tp::Repr::CplxRe1Im1;
constexpr auto kFpI16AbRawF32ReprHannMatrix = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw;
constexpr auto kFpI16AbRawF32ReprAb = Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint | Tp::Repr::CplxRe1Im1;
constexpr auto kFpI16AbRawF32ReprAbDivIntermediary = Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint;
constexpr auto kFpI16AbRawF32ReprGauss = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw | Tp::Repr::CplxRe1Im1;
constexpr auto kFpI16AbRawF32ReprEta = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw;

using FpI16RawF32OpsBase = CommonOps<
	kFpI16AbRawF32ReprBuffer,
	kFpI16AbRawF32ReprHannMatrix,
	kFpI16AbRawF32ReprAb,
	kFpI16AbRawF32ReprAbDivIntermediary,
	kFpI16AbRawF32ReprGauss,
	kFpI16AbRawF32ReprEta,
	true>;

/// \brief A naive implementation designed to test the implementation, and to serve as a starting point for
/// implementing other memory-optimized variants of replaceable Ops instances.
///
class FpI16AbRawF32Ops : public FpI16RawF32OpsBase {
public:
	void fft2(void *aBufferComplex) override;
	void ifft2(void *aBufferComplex) override;
	FpI16AbRawF32Ops();
	void initImpl() override;
	const void *hannMatrix() override;  ///< Precompiled hann matrix
	const void *gaussFft() override;  ///< Precompiled scaled fourier-transformed Gaussian matrix
private:
	PrecompiledMatrixHelper<const float> pmHelper;
	Fft fft;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_OPS_FPI16ABRAWF32OPS_HPP_
