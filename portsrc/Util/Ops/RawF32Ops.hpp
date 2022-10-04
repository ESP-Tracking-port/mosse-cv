//
// RawF32Ops.hpp
//
// Created on: Sep 05, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_OPS_RAWF32OPS_HPP_)
#define MOSSE_OPS_RAWF32OPS_HPP_

#include "portsrc/Util/Helper/PrecompiledMatrixHelper.hpp"
#include "portsrc/Util/Ops/CommonOps.hpp"

class Fft;

namespace Mosse {
namespace Ut {

constexpr auto kRawF32ReprBuffer = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw | Tp::Repr::CplxRe1Im1;
constexpr auto kRawF32ReprHannMatrix = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw;
constexpr auto kRawF32ReprAb = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw | Tp::Repr::CplxRe1Im1;
constexpr auto kRawF32ReprAbDivIntermediary = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw;
constexpr auto kRawF32ReprGauss = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw | Tp::Repr::CplxRe1Im1;
constexpr auto kRawF32ReprEta = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw;

using RawF32OpsBase = CommonOps< kRawF32ReprBuffer, kRawF32ReprHannMatrix, kRawF32ReprAb, kRawF32ReprAbDivIntermediary,
	kRawF32ReprGauss, kRawF32ReprEta, true>;

/// \brief A naive implementation designed to test the implementation, and to serve as a starting point for
/// implementing other memory-optimized variants of replaceable Ops instances.
///
class RawF32Ops : public RawF32OpsBase {
public:
	void fft2(void *aBufferComplex) override;
	void ifft2(void *aBufferComplex) override;
	RawF32Ops();
	void initImpl() override;
	const void *hannMatrix() override;  ///< Precompiled hann matrix
	const void *gaussFft() override;  ///< Precompiled scaled fourier-transformed Gaussian matrix
private:
	/// \brief Boilerplate-reducing method
	/// \arg aFwd Performs fft, if true, ifft otherwise.
	///
	void fft2Common(void *aBufferComplex, bool aFwd);
private:
	PrecompiledMatrixHelper<const float> pmHelper;
	Fft fft;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_OPS_RAWF32OPS_HPP_
