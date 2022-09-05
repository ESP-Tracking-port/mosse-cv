//
// EigenMem.hpp
//
// Created on: Sep 01, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_HELPER_EIGENMEM_HPP_)
#define MOSSE_UTIL_HELPER_EIGENMEM_HPP_

#include "Types/EigenAux.hpp"
#include "Types/Repr.hpp"
#include "Types/Tracking.hpp"
#include "Util/Arithm/MemLayout.hpp"
#include "Util/Helper/En.h"

namespace Mosse {
namespace Ut {

template <Tp::Repr::Flags F>
inline typename Tp::template EigenMapType<F>::Type makeEigenMap(void *bufferComplexOrReal, const Tp::Roi &roi)
{
	const auto strideOuter = Ut::strideInner<F>() * roi.size.cols();

	return typename Tp::EigenMapType<F>::Type{static_cast<ReTp<F> *>(Ut::at<F>(Ut::offsetFirstReal<F>(roi),
		bufferComplexOrReal)), roi.size.rows(), roi.size.cols(), typename Tp::EigenMapType<F>::StrideType{
		Ut::strideInner<F>(), strideOuter}};
}

template <Tp::Repr::Flags F>
inline const typename Tp::template EigenMapType<F>::Type makeEigenMap(const void *bufferComplexOrReal,
	const Tp::Roi &roi)
{
	const auto strideOuter = Ut::strideInner<F>() * roi.size.cols();

	return typename Tp::EigenMapType<F>::Type{static_cast<const ReTp<F> *>(Ut::at<F>(Ut::offsetFirstReal<F>(roi),
		bufferComplexOrReal)), roi.size.rows(), roi.size.cols(), typename Tp::EigenMapType<F>::StrideType{
		Ut::strideInner<F>(), strideOuter}};
}

template <Tp::Repr::Flags F>
inline typename Tp::EigenMapType<F>::Type makeEigenMapImag(void *bufferComplex, const Tp::Roi &roi,
	En<F & Tp::Repr::MaskCplx> = nullptr)
{
	const auto strideOuter = Ut::strideInner<F>() * roi.size.cols();

	return typename Tp::EigenMapType<F>::Type{static_cast<ReTp<F> *>(Ut::at<F>(Ut::offsetFirstImag<F>(roi),
		bufferComplex)), roi.size.rows(), roi.size.cols(), typename Tp::EigenMapType<F>::StrideType{
		Ut::strideInner<F>(), strideOuter}};
}

template <Tp::Repr::Flags F>
inline const typename Tp::EigenMapType<F>::Type makeEigenMapImag(const void *bufferComplex, const Tp::Roi &roi,
	En<F & Tp::Repr::MaskCplx> = nullptr)
{
	const auto strideOuter = Ut::strideInner<F>() * roi.size.cols();

	return typename Tp::EigenMapType<F>::Type{static_cast<const ReTp<F> *>(Ut::at<F>(Ut::offsetFirstImag<F>(roi),
		bufferComplex)), roi.size.rows(), roi.size.cols(), typename Tp::EigenMapType<F>::StrideType{Ut::strideInner<F>(),
		strideOuter}};
}

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_HELPER_EIGENMEM_HPP_
