//
// Arithm.hpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_ARITHM_HPP_)
#define MOSSE_UTIL_ARITHM_HPP_

#include "Types/Repr.hpp"
#include "Util/Helper/En.h"
#include "Util/Helper/ReTp.hpp"
#include "Util/Arithm/Conv.hpp"

namespace Mosse {
namespace Ut {
namespace Impl {

/// \brief Default implementation, just converts from representation to floats, and performs multiplication.
/// Optimizations will be achieved through template specialization.
///
template <Tp::Repr::Flags R1, Tp::Repr::Flags R2, Tp::Repr::Flags O>
struct MulCplxA3 {
	static inline void call(ReTp<R1> aRe1, ReTp<R1> aIm1, ReTp<R2> aRe2, ReTp<R2> aIm2, ReTp<O> &aoRe, ReTp<O> &aoIm)
	{
		auto aRe1f = fromRepr<float, R1>(aRe1);
		auto aIm1f = fromRepr<float, R1>(aIm1);
		auto aRe2f = fromRepr<float, R2>(aRe2);
		auto aIm2f = fromRepr<float, R2>(aIm2);
		float oRef = aRe1f * aRe2f - aIm1f * aIm2f;
		float oImf = aRe1f * aIm2f + aIm1f * aRe2f;
		aoRe = toRepr<O>(oRef);
		aoIm = toRepr<O>(oImf);
	}
};

}  // namespace Impl

template <Tp::Repr::Flags R1, Tp::Repr::Flags R2, Tp::Repr::Flags O>
inline void mulCplxA3(ReTp<R1> aRe1, ReTp<R1> aIm1, ReTp<R2> aRe2, ReTp<R2> aIm2, ReTp<O> &aoRe, ReTp<O> &aoIm)
{
	Impl::MulCplxA3<R1 & Tp::Repr::MaskTraitScalar, R2 & Tp::Repr::MaskTraitScalar,
		O & Tp::Repr::MaskTraitScalar>::call(aRe1, aIm1, aRe2, aIm2, aoRe, aoIm);
}

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_ARITHM_HPP_
