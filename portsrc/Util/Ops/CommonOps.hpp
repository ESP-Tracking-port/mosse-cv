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
	Tp::Repr::Flags ReprGauss>
class CommonOps : public Ops {
public:
	static_assert(ReprBuffer & (Tp::Repr::CplxRe1Im1 | Tp::Repr::CplxRenImn), "");
	static_assert(Tp::Repr::isValid<ReprBuffer>(), "");
	static_assert(Tp::Repr::isValid<ReprHann>(), "");

	/// \brief Initializes the buffer by splitting the image into its real and imaginary (zeroed) part
	///
	void imageCropInto(Tp::Image aImageReal, void *aBufferComplex) override
	{
		bufferComplexInit<ReprBuffer>(aImageReal, aBufferComplex);
	}

	void maxReal(const void *aComplexBuffer, Tp::PointRowCol &aPos, float *sum) override
	{
		maxReal<ReprBuffer>(aComplexBuffer, aPos, sum);
	}

	void imagePreprocess(void *aCropBufferComplex) override
	{
		imagePreprocess<ReprBuffer, ReprHann>(aCropBufferComplex);
	}

	void imageConvFftDomain(void *aioCropFft2Complex, void *aMatrixAcomlex, void *aMatrixBcomplex) override
	{
		auto mapFft = Ut::makeEigenMap<ReprBuffer>(aioCropFft2Complex, roi());
		auto mapFftImag = Ut::makeEigenMap<ReprBuffer>(aioCropFft2Complex, roi());
		auto mapA = Ut::makeEigenMap<ReprAb>(aMatrixAcomlex, roi());
		auto mapAimag = Ut::makeEigenMapImag<ReprAb>(aMatrixAcomlex, roi());
		auto mapB = Ut::makeEigenMap<ReprAb>(aMatrixBcomplex, roi());
		auto mapBimag = Ut::makeEigenMapImag<ReprAb>(aMatrixBcomplex, roi());
		// Intermediate buffers
		ReTp<ReprAbHookIntermDiv> hRe;  // H = divCplx(A, B)
		ReTp<ReprAbHookIntermDiv> hIm;

		for (unsigned row = 0; row < roi().size.rows(); ++row) {
			for (unsigned col = 0; col < roi().size.cols(); ++col) {
				Ut::divCplxA3<ReprAb, ReprAb, ReprAbHookIntermDiv>(mapA(row, col), mapAimag(row, col), mapB(row, col),
					mapBimag(row, col), hRe, hIm);
				Ut::mulCplxA3<ReprBuffer, ReprAbHookIntermDiv, ReprBuffer>(mapFft(row, col), mapFftImag(row, col), hRe,
					hIm, mapFft(row, col), mapFftImag(row, col));
			}
		}
	}

	float calcPsr(const void *aComplexBuffer, const Tp::PointRowCol &aPeak, float sumHint,
		Tp::PointRowCol aMask) override
	{
		// Mean value excluding the masked (usually, 11x11 field) around the peak
		auto map = Ut::makeEigenMap<ReprBuffer>(aComplexBuffer, roi());
		float mean = 0.0f;
		Tp::Roi roiMask{{roi().origin + aPeak - (aMask / 2)}, aMask};
		const float sizeMasked = static_cast<float>(roi().area() - roiMask.area());

		{
			auto mapMask = map.block(roiMask.origin(0), roiMask.origin(1), roiMask.size(0), roiMask.size(1));
			FloatSumVisitor<ReTp<ReprBuffer>, ReprBuffer> visitor;
			mapMask.visit(visitor);
			sumHint -= visitor.sum;
			mean = sumHint / sizeMasked;
		}

		float stddev = 0.0f;
		{
			FloatDevSumVisitor<ReprBuffer, true> visitor{0.0f, mean, roiMask};
			map.visit(visitor);
			stddev = visitor.devsum / sqrt(sizeMasked);
		}

		float maxValue = Ut::fromRepr<float, ReprBuffer>(map(aPeak(0), aPeak(1)));
		float psr = (maxValue - mean) / stddev;

		return psr;
	}

	virtual void mataUpdate(void *aMatAcomplex, const void *aImageCropFftComplex, float aEta, bool aInitial) override
	{
		auto mapA = Ut::makeEigenMap<ReprAb>(aMatAcomplex, roi());
		auto mapAimag = Ut::makeEigenMapImag<ReprAb>(aMatAcomplex, roi());
		auto mapFft = Ut::makeEigenMap<ReprBuffer>(aImageCropFftComplex, roi());
		auto mapFftImag = Ut::makeEigenMapImag<ReprBuffer>(aImageCropFftComplex, roi());
		auto mapGauss = Ut::makeEigenMap<ReprGauss>(aImageCropFftComplex, roi());
		auto mapGaussImag = Ut::makeEigenMap<ReprGauss>(aImageCropFftComplex, roi());
	}
private:

	/// \brief Unlike the workflow implied by the API, performs both buffer initialization and image preprocessing,
	/// because there are certain points which can be optimized by sparing conversions.
	///
	/// \pre Hann matrix uses raw float representation
	///
	template <Tp::Repr::Flags F>
	void bufferComplexInit(Tp::Image aImage, void *aBufferCplx)
	{
		auto map = makeEigenMap<F>(aBufferCplx, roi());
		auto mapImag = makeEigenMapImag<F>(aBufferCplx, roi());
		auto blockImage = aImage.block(roi().origin(0), roi().origin(1), roi().size(0), roi().size(1));
		float *logTable = Mosse::getLogTable8bit();
		float sum = 0.0f;

		// Calculating mean value

		for (unsigned row = 0; row < map.rows(); ++row) {
			for (unsigned col = 0; col < map.cols(); ++col) {
				sum += logTable[blockImage(row, col)];
			}
		}

		const float mean = sum / static_cast<float>(map.size());
		float devsum = 0.0f;

		// Second turn: calculating standard deviation

		for (unsigned row = 0; row < map.rows(); ++row) {
			for (unsigned col = 0; col < map.cols(); ++col) {
				devsum += abs(mean - logTable[blockImage(row, col)]);
			}
		}

		// Final turn: initializing the array

		const float stddev = devsum / sqrt(static_cast<float>(map.size()));
		auto mapHann = makeEigenMap<ReprHann>(hannMatrix(), roi());

		for (unsigned row = 0; row < map.rows(); ++row) {
			for (unsigned col = 0; col < map.cols(); ++col) {
				constexpr float kEps = 1e-5;  // Small fraction to prevent zero division
				mapImag(row, col) = toRepr<F>(0);
				float pixel = (logTable[blockImage(row, col)] - mean) / (stddev + kEps)
					* fromRepr<ReprHann>(mapHann(row, col));  // Log table is an optimization shortcut. The log(0) issue is already taken care of during the table compilation stage.
				map(row, col) = toRepr<F>(pixel);
			}
		}
	}

	/// \brief Finds the max element
	///
	/// \pre (1) It must be raw representation,
	/// \pre (2) The array elements are expected to be unaligned
	/// \pre (3) Row-major array is expected (elements are addressed as array[row][col])
	///
	template <Tp::Repr::Flags F>
	inline void maxReal(const void *aComplexBuffer, Tp::PointRowCol &aPos, float *sum)
	{
		static constexpr auto kStrideInner = Ut::strideInner<F>();
		const auto strideOuter = kStrideInner * roi().size.cols;
		using ValueType = typename Tp::Repr::template Type<F>;
		typename Tp::EigenMapType<F>::Type map{aComplexBuffer, roi().size.rows, roi().size.cols,
			typename Tp::EigenMapType<F>::StrideType{kStrideInner, strideOuter}};

		if (nullptr == sum) {
			MaxVisitor<ValueType, F> visitor;
			map.visit(visitor);
			aPos = visitor.pos;
		} else {
			CompositeVisitor<MaxVisitor<ValueType, F>, FloatSumVisitor<ValueType, F>> visitor;
			map.visit(visitor);
			*sum = visitor.template get<1>().sum;
			aPos = visitor.template get<0>().pos;
		}
	}

	template <Tp::Repr::Flags B, Tp::Repr::Flags H>
	inline void imagePreprocess(void *)
	{
	}
};

}  // namespace Ut
}  // namespace Mosse

#endif  // MOSSE_UTIL_OPS_COMMONOPS_HPP_
