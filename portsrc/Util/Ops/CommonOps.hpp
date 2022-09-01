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
#include "Util/Arithm/Arithm.hpp"
#include "Util/Arithm/MemLayout.hpp"
#include "Util/Ops.hpp"
#include "Util/Helper/EigenVisitor.hpp"
#include "Util/Helper/EigenMem.hpp"
#include <Eigen/Core>
#include <type_traits>
#include <cassert>

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
/// \tparam ReprHann Representation flags for Hann matrices
///
template <Tp::Repr::Flags ReprBuffer, Tp::Repr::Flags ReprHann>
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
private:
	template <Tp::Repr::Flags F>
	inline typename Tp::EigenMapType<F>::Type makeMap(void *aBufferCplx)
	{
		const auto strideOuter = Ut::strideInner<F>() * roi().size.cols();
		return typename Tp::EigenMapType<F>::Type{aBufferCplx, roi().size.rows(), roi().size.cols(),
			typename Tp::EigenMapType<F>::StrideType{Ut::strideInner<F>(), strideOuter}};
	}

	template <Tp::Repr::Flags F>
	void bufferComplexInit(Tp::Image aImage, void *aBufferCplx)
	{
		auto map = makeEigenMapReal<F>(aBufferCplx, roi());
		auto mapImag = makeEigenMapImag<F>(aBufferCplx, roi());

		for (unsigned row = 0; row < map.rows(); ++row) {
			for (unsigned col = 0; col < map.cols(); ++col) {
				map(row, col) = toRepr<F>(aImage(row, col));
				mapImag(row, col) = toRepr<F>(0);
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
	inline void imagePreprocess(void *aComplexBuffer)
	{
		(void)aComplexBuffer;
	}
};

}  // namespace Ut
}  // namespace Mosse

#endif  // MOSSE_UTIL_OPS_COMMONOPS_HPP_
