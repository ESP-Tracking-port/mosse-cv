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

namespace Mosse {
namespace Ut {

template <Tp::Repr::Flags F>
inline typename Tp::EigenMapType<F>::Type makeEigenMapReal(void *bufferComplex, Tp::Roi roi)
{
	const auto strideOuter = Ut::strideInner<F>() * roi.size.cols();

	return typename Tp::EigenMapType<F>::Type{Ut::at(Ut::offsetFirstReal(roi), bufferComplex),
		roi.size.rows(), roi.size.cols(), typename Tp::EigenMapType<F>::StrideType{Ut::strideInner<F>(), strideOuter}};
}

template <Tp::Repr::Flags F>
inline typename Tp::EigenMapType<F>::Type makeEigenMapImag(void *bufferComplex, Tp::Roi roi)
{
	const auto strideOuter = Ut::strideInner<F>() * roi.size.cols();

	return typename Tp::EigenMapType<F>::Type{Ut::at(Ut::offsetFirstImag(roi), bufferComplex),
		roi.size.rows(), roi.size.cols(), typename Tp::EigenMapType<F>::StrideType{Ut::strideInner<F>(), strideOuter}};
}

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_HELPER_EIGENMEM_HPP_
