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
#include <Eigen/Core>
#include <type_traits>
#include <cassert>

namespace Mosse {
namespace Ut {

template <Tp::Repr::Flags kReprBuffer>
class CommonOps : public Ops {
private:
	template <bool F>
	using En = typename std::enable_if<F>::type *;
public:
	static_assert(kReprBuffer & (Tp::Repr::CplxRe1Im1 | Tp::Repr::CplxRenImn), "");

	/// \brief Initializes the buffer by splitting the image into its real and imaginary (zeroed) part
	///
	void imageCropInto(Tp::Image aImageReal, void *aBufferComplex) override
	{
		const auto cols = aImageReal.cols;
		const auto rows = aImageReal.rows;
		bufferComplexInitRe(aImageReal, aBufferComplex);
		bufferComplexInitImZeros(aBufferComplex);
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
	void bufferComplexInitRe(Tp::Image aImage, void *aBufferCplx)
	{
		using ValueType = typename Tp::Repr::template Type<F>;
		auto map = makeMap(aBufferCplx);

		for (unsigned row = 0; row < map.rows(); ++row) {
			for (unsigned col = 0; col < map.cols(); ++col) {
				map(row, col) = toRepr<F>(aImage(row, col));
			}
		}
	}

	/// \brief Re1 Im1 order, short representation (length is less than that of the CPU's registers)
	///
	template <Tp::Repr::Flags F>
	void bufferComplexInitImZeros(void *aBufferCplx,
		En<(F & Tp::Repr::CplxRe1Im1) && (szof<F>() < sizeof(std::size_t))> = nullptr)
	{
		constexpr auto kReprSizeof = szof<kReprBuffer>();
		using ValueType = typename Tp::Repr::template Type<F>;
		constexpr auto maskAb = ~Ut::maskAb<ValueType, kReprSizeof>();
		const auto signalLength = roi().size.cols() * roi().size.rows();
		const auto bufferSize = signalLength * kReprSizeof * 2 / sizeof(std::size_t);
		assert(bufferSize % sizeof(std::size_t) == 0);

		for (std::size_t i = 0; i < bufferSize; i += sizeof(std::size_t)) {
			*(static_cast<std::size_t *>(aBufferCplx) + i) &= maskAb;
		}
	}

	/// \brief Re1 Im1 order, long representation (equal to the length of the CPU's registers)
	///
	template <Tp::Repr::Flags F>
	void bufferComplexInitImZeros(void *aBufferCplx,
		En<(F & Tp::Repr::CplxRe1Im1) && (szof<F>() == sizeof(std::size_t))> = nullptr)
	{
		const auto signalLength = roi().size.cols() * roi().size.rows();
		const auto bufferSize = signalLength * 2;

		for (std::size_t i = 1; i < bufferSize; i += sizeof(std::size_t)) {
			*(static_cast<std::size_t *>(aBufferCplx) + i) = 0;
		}
	}

	/// \brief Finds the max element
	///
	/// \pre (1) It must be raw representation,
	/// \pre (2) The array elements are expected to be unaligned
	/// \pre (3) Row-major array is expected (elements are addressed as array[row][col])
	///
	template <Tp::Repr::Flags F>
	void maxReal(const void *aComplexBuffer, Tp::PointRowCol &aPos, float *sum)
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
};

}  // namespace Ut
}  // namespace Mosse

#endif  // MOSSE_UTIL_OPS_COMMONOPS_HPP_
