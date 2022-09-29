//
// CommonOps.hpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_OPS_COMMONOPS_HPP_)
#define MOSSE_UTIL_OPS_COMMONOPS_HPP_

#include "Types/Tracking.hpp"
#include "Types/Repr.hpp"
#include "Types/EigenAux.hpp"
#include "Util/Arithm/Conv.hpp"
#include "Util/Arithm/MemLayout.hpp"
#include "Util/Arithm/Arithm.hpp"
#include "Util/Ops.hpp"
#include "Util/Helper/EigenVisitor.hpp"
#include "Util/Helper/EigenMem.hpp"
#include "Util/Helper/ReTp.hpp"
#include "Port/MossePort.hpp"
#include "MosseApi.hpp"
#include <Eigen/Core>
#include <type_traits>
#include <cassert>
#include <cmath>

namespace Mosse {
namespace Ut {

/// \brief Serves as a boilerplate-reducing intermediary between the API (virtual method) and a particular
/// implementation of the tracker.
///
/// \details The implementation is altered in compile time by use of representation flags which define representation,
/// memory layout, and which FFT and arithmetic operations depend upon.
///
/// \tparam ReprBuffer Representation flag for intermediary buffer in which FFT and convolution calculations are
/// performed
///
/// \tparam ReprHann Representation flags for Hann matrices. Present among template arguments, but not used yet. Point
/// of extension, if the necessity comes
///
/// \tparam ReprAb Representation for A and B matrices.
///
/// \tparam ReprAbHookIntermDiv Optimization hook. Convolution is performed as `mulElementWise(H, divCplx(A, B))`. Since
/// using some common intermediate representation or making a redundant conversion may be inefficient, we leave the
/// possibility to store intermediary in some easy-to-compute representation scheme. For example, it may be useful for
/// multiplying fixed-point.
///
/// Example (simplified, not relevant to the convolution):
///
/// fixedOffset = 5
/// hd = h(row, col) / fixedOffset * 10
/// ad = a(row, col) / (fixedOffset * 10)
/// bd = b(row, col) / (fixedOffset * 10)
/// intermediateDenormalized = ad * bd
/// hd *= itermediateDenormalized
/// hd *= (int)(fixedOffset * 10)
///
/// Without this hook, multiplication of a and b would first be converted in normalized fixed point, then denormalized,
/// multiplied by H, and normalized again. An additional normalization / denormalizaton takes place in this case.
///
/// \tparam ReprGauss Representation for the Fourier-transformed gaussian kernel
///
template <
	Tp::Repr::Flags ReprBuffer,
	Tp::Repr::Flags ReprHann,
	Tp::Repr::Flags ReprAb,
	Tp::Repr::Flags ReprAbHookIntermDiv,
	Tp::Repr::Flags ReprGauss,
	Tp::Repr::Flags ReprEta,
	bool ScaledGauss = false>
class CommonOps : public DecomposedOps {
public:
	static constexpr struct {
		Tp::Repr::Flags buffer;
		Tp::Repr::Flags matAb;
	} reprFlags {ReprBuffer, ReprAb};

	static_assert(ReprBuffer & (Tp::Repr::CplxRe1Im1 | Tp::Repr::CplxRenImn), "");
	static_assert(Tp::Repr::isValid<ReprBuffer>(), "");
	static_assert(Tp::Repr::isValid<ReprHann>(), "");
	static_assert(Tp::Repr::isValid<ReprAb>(), "");
	static_assert(Tp::Repr::isValid<ReprAbHookIntermDiv>(), "");
	static_assert(Tp::Repr::isValid<ReprGauss>(), "");
	static_assert(Tp::Repr::isValid<ReprEta>(), "");

	/// \brief Finds the max element
	///
	/// \pre (1) It must be raw representation,
	/// \pre (2) The array elements are expected to be unaligned
	/// \pre (3) Row-major array is expected (elements are addressed as array[row][col])
	///
	void maxReal(const void *aComplexBuffer, Tp::PointRowCol &aPos, float *sum) override
	{
		auto map = Ut::makeEigenMap<ReprBuffer>(aComplexBuffer, roi());
		auto mapBlock = Ut::makeEigenBlock(map, roiFragment());

		if (nullptr == sum) {
			MaxVisitor<ReprBuffer> visitor;
			mapBlock.visit(visitor);
			aPos = visitor.pos;
		} else {
			CompositeVisitor<ReprBuffer, MaxVisitor<ReprBuffer>, FloatSumVisitor<ReprBuffer>> visitor;
			mapBlock.visit(visitor);
			*sum = visitor.template get<1>().sum;
			aPos = visitor.template get<0>().pos;
		}

		aPos += roiFragment().origin;
	}

	void imagePreprocess(void *) override
	{
	}

	void imageConvFftDomain(void *aioCropFft2Complex, void *aMatrixAcomplex, void *aMatrixBcomplex) override
	{
		auto mapFft = Ut::makeEigenMap<ReprBuffer>(aioCropFft2Complex, roi());
		auto mapFftImag = Ut::makeEigenMapImag<ReprBuffer>(aioCropFft2Complex, roi());
		auto mapA = Ut::makeEigenMap<ReprAb>(aMatrixAcomplex, roi());
		auto mapAimag = Ut::makeEigenMapImag<ReprAb>(aMatrixAcomplex, roi());
		auto mapB = Ut::makeEigenMap<ReprAb>(aMatrixBcomplex, roi());
		auto mapBimag = Ut::makeEigenMapImag<ReprAb>(aMatrixBcomplex, roi());
		// Intermediate buffers
		ReTp<ReprAbHookIntermDiv> hRe;  // H = divCplx(A, B)
		ReTp<ReprAbHookIntermDiv> hIm;

		for (auto row = roiFragment().origin(0); row < roiFragment().origin(0) + roiFragment().size(0); ++row) {
			for (auto col = roiFragment().origin(1); col < roiFragment().origin(1) + roiFragment().size(1); ++col) {
				Ut::divCplxA3<ReprAb, ReprAb, ReprAbHookIntermDiv>(mapA(row, col), mapAimag(row, col), mapB(row, col),
					mapBimag(row, col), hRe, hIm);
				Ut::mulCplxA3<ReprBuffer, ReprAbHookIntermDiv, ReprBuffer>(mapFft(row, col), mapFftImag(row, col), hRe,
					hIm, mapFft(row, col), mapFftImag(row, col));
			}
		}
	}

	float bufferAtAsFloat(const void *aComplexBuffer, const Tp::PointRowCol &aPeak) override
	{
		auto ret = Ut::fromRepr<float, ReprBuffer>(Ut::atAsVariant<ReprBuffer>(aPeak, roi(), aComplexBuffer));

		return ret;
	}

	float bufferSum(const void *aComplexBuffer, const Tp::Roi &aRoi)
	{
		auto r = aRoi;
		auto map = Ut::makeEigenMap<ReprBuffer>(aComplexBuffer, roi());
		auto mapBlock = Ut::makeEigenBlock(map, r);
		FloatSumVisitor<ReprBuffer> visitor;
		mapBlock.visit(visitor);

		return visitor.sum;
	}

	virtual float bufferAbsDevSum(const void *aComplexBuffer, const Tp::Roi &aRoi, float aMean) override
	{
		auto r = aRoi;
		auto map = Ut::makeEigenMap<ReprBuffer>(aComplexBuffer, roi());
		auto mapBlock = Ut::makeEigenBlock(map, r);
		FloatDevSumVisitor<ReprBuffer, false> visitor{0.0f, aMean, {}};
		mapBlock.visit(visitor);

		return visitor.devsum;
	}

	template <bool S = ScaledGauss>
	inline typename std::enable_if<!S>::type gaussScale(ReTp<ReprGauss> &aGauss)
	{
		Ut::mulA3<ReprEta, ReprGauss, ReprGauss>(eta(), aGauss, aGauss);
	}

	template <bool S = ScaledGauss>
	inline typename std::enable_if<S>::type gaussScale(ReTp<ReprGauss> &)
	{
	}

	/// \brief
	///
	/// \pre It is expected that Gaussian matrix is already multiplied by eta
	///
	virtual void mataUpdate(void *aMatAcomplex, const void *aImageCropFftComplex, bool aInitial) override
	{
		auto mapA = Ut::makeEigenMap<ReprAb>(aMatAcomplex, roi());
		auto mapAimag = Ut::makeEigenMapImag<ReprAb>(aMatAcomplex, roi());
		auto mapFft = Ut::makeEigenMap<ReprBuffer>(aImageCropFftComplex, roi());
		auto mapFftImag = Ut::makeEigenMapImag<ReprBuffer>(aImageCropFftComplex, roi());
		auto mapGauss = Ut::makeEigenMap<ReprGauss>(gaussFft(), roi());
		auto mapGaussImag = Ut::makeEigenMapImag<ReprGauss>(gaussFft(), roi());

		for (auto row = roiFragment().origin(0); row < roiFragment().origin(0) + roiFragment().size(0); ++row) {
			for (auto col = roiFragment().origin(1); col < roiFragment().origin(1) + roiFragment().size(1); ++col) {
				auto gauss = mapGauss(row, col);
				auto gaussIm = mapGaussImag(row, col);
				auto aPrev = mapA(row, col);
				auto aImPrev = mapAimag(row, col);
				auto frameFftImag = Ut::minus<ReprBuffer>(mapFftImag(row, col));  // Complex conjugate. See the mosse paper.

				gaussScale(gauss);  // Multiplication by $\eta$ (if a pre-multiplied filter is not used yet). See the mosse paper
				gaussScale(gaussIm);

				Ut::mulCplxA3<ReprGauss, ReprBuffer, ReprAb>(gauss, gaussIm, mapFft(row, col), frameFftImag,
					mapA(row, col), mapAimag(row, col));

				if (!aInitial) {
					Ut::mulA3<ReprEta, ReprAb, ReprAb>(invEta(), aPrev, aPrev);
					Ut::mulA3<ReprEta, ReprAb, ReprAb>(invEta(), aImPrev, aImPrev);
					Ut::sumA3<ReprAb, ReprAb, ReprAb>(mapA(row, col), aPrev, mapA(row, col));
					Ut::sumA3<ReprAb, ReprAb, ReprAb>(mapAimag(row, col), aImPrev, mapAimag(row, col));
				}
			}
		}
	}

	virtual void matbUpdate(void *aMatBcomplex, const void *aImageCropFftComplex, bool aInitial)
	{
		auto mapB = Ut::makeEigenMap<ReprAb>(aMatBcomplex, roi());
		auto mapBimag = Ut::makeEigenMapImag<ReprAb>(aMatBcomplex, roi());
		auto mapFft = Ut::makeEigenMap<ReprBuffer>(aImageCropFftComplex, roi());
		auto mapFftImag = Ut::makeEigenMapImag<ReprBuffer>(aImageCropFftComplex, roi());

		for (auto row = roiFragment().origin(0); row < roiFragment().origin(0) + roiFragment().size(0); ++row) {
			for (auto col = roiFragment().origin(1); col < roiFragment().origin(1) + roiFragment().size(1); ++col) {
				auto frameFftImConj = Ut::minus<ReprBuffer>(mapFftImag(row, col));
				auto bPrev = mapB(row, col);
				auto bImPrev = mapB(row, col);

				// B = complexMultiplication(fft(imageCrop), complexConjugate(fft(imageCrop)))
				Ut::mulCplxA3<ReprBuffer, ReprBuffer, ReprAb>(mapFft(row, col), mapFftImag(row, col), mapFft(row, col),
					frameFftImConj, mapB(row, col), mapBimag(row, col));

				if (!aInitial) {
					// B' = eta * complexMultiplication(fft(imageCrop), complexConjugate(fft(imageCrop)))
					Ut::mulA3<ReprEta, ReprAb, ReprAb>(eta(), mapB(row, col),
						mapB(row, col));
					Ut::mulA3<ReprEta, ReprAb, ReprAb>(eta(), mapBimag(row, col),
						mapBimag(row, col));
					// B(t-1) * (1 - eta), both real and complex plane
					Ut::mulA3<ReprEta, ReprAb, ReprAb>(invEta(), bPrev, bPrev);
					Ut::mulA3<ReprEta, ReprAb, ReprAb>(invEta(), bImPrev, bImPrev);
					// B(t) := B' + B(t-1) * (1 - eta), both real and complex plane
					Ut::sumA3<ReprAb, ReprAb, ReprAb>(mapB(row, col), bPrev, mapB(row, col));
					Ut::sumA3<ReprAb, ReprAb, ReprAb>(mapBimag(row, col), bImPrev, mapBimag(row, col));
				}
			}
		}
	}

	void imageCropPreprocessImpl(Tp::Image aImage, void *aBufferCplx, Tp::NumVariant mean, Tp::NumVariant stddev) override
	{
		auto map = makeEigenMap<ReprBuffer>(aBufferCplx, roi());
		auto mapImag = makeEigenMapImag<ReprBuffer>(aBufferCplx, roi());
		auto blockImage = Ut::makeEigenBlock(aImage, roi());
		const float *logTable = Mosse::getLogTable8bit();
		auto mapHann = makeEigenMap<ReprHann>(hannMatrix(), roi());

		for (auto row = roiFragment().origin(0); row < roiFragment().origin(0) + roiFragment().size(0); ++row) {
			for (auto col = roiFragment().origin(1); col < roiFragment().origin(1) + roiFragment().size(1); ++col) {
				constexpr float kEps = 1e-5;  // Small fraction to prevent zero division
				mapImag(row, col) = toRepr<ReprBuffer>(0.0f);
				float pixel = (logTable[blockImage(row, col)] - mean.f32) / (stddev.f32 + kEps)
					* fromRepr<float, ReprHann>(mapHann(row, col));  // Log table is an optimization shortcut. The log(0) issue is already taken care of during the table compilation stage.
				map(row, col) = toRepr<ReprBuffer>(pixel);
				mosseassertnotnan(CommonOps::imageCropInto, map(row, col), row, col);
				mosseassertnotnan(CommonOps::imageCropInto, mapImag(row, col), row, col);
			}
		}
	}
protected:
	virtual const void *hannMatrix() = 0;  ///< Precompiled or generated (for test implementations) hann matrix
	virtual const void *gaussFft() = 0;  ///< Fouried-transformed precompiled or generated Gaussian matrix
};

}  // namespace Ut
}  // namespace Mosse

#endif  // MOSSE_UTIL_OPS_COMMONOPS_HPP_
