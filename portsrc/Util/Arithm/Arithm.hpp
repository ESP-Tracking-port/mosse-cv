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
#include "MossePort.hpp"

namespace Mosse {
namespace Ut {
namespace Impl {

// Complex multiplication

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

namespace Impl {

// Complex division

/// \brief Complex division
///
template <Tp::Repr::Flags R1, Tp::Repr::Flags R2, Tp::Repr::Flags O>
struct DivCplxA3 {
	static inline void call(ReTp<R1> aRe1, ReTp<R1> aIm1, ReTp<R2> aRe2, ReTp<R2> aIm2, ReTp<O> &aoRe, ReTp<O> &aoIm)
	{
		auto a = fromRepr<float, R1>(aRe1);
		auto b = fromRepr<float, R1>(aIm1);
		auto c = fromRepr<float, R2>(aRe2);
		auto d = fromRepr<float, R2>(aIm2);
		float oRef = (a * c + b * d) / (c * c + d * d);
		float oImf = (b * c - a * d) / (c * c + d * d);
		aoRe = toRepr<O>(oRef);
		aoIm = toRepr<O>(oImf);
		ohdebugsectif(std::isnan(oRef) || std::isnan(oImf), {
			ohdebug(DivCplxA3::call, R1, R2, O, oRef, oImf);
			assert(false);
		});
	}
};

}  // namespace Impl

template <Tp::Repr::Flags R1, Tp::Repr::Flags R2, Tp::Repr::Flags O>
inline void divCplxA3(ReTp<R1> aRe1, ReTp<R1> aIm1, ReTp<R2> aRe2, ReTp<R2> aIm2, ReTp<O> &aoRe, ReTp<O> &aoIm)
{
	Impl::DivCplxA3<R1 & Tp::Repr::MaskTraitScalar, R2 & Tp::Repr::MaskTraitScalar,
		O & Tp::Repr::MaskTraitScalar>::call(aRe1, aIm1, aRe2, aIm2, aoRe, aoIm);
}

// Minus

namespace Impl {

template <Tp::Repr::Flags F>
struct Minus {
	static inline ReTp<F> call(ReTp<F> a)
	{
		return -a;
	}
};

}  // namespace Impl

template <Tp::Repr::Flags F>
inline ReTp<F> minus(ReTp<F> a)
{
	return Impl::Minus<F & Tp::Repr::MaskTraitScalar>::call(a);
}

// mult

namespace Impl {

template <Tp::Repr::Flags R1, Tp::Repr::Flags R2, Tp::Repr::Flags O>
struct MulA3 {
	static inline void call(ReTp<R1> a1, ReTp<R2> a2, ReTp<O> &aOut)
	{
		float a1f = fromRepr<float, R1>(a1);
		float a2f = fromRepr<float, R2>(a2);
		float aOutf = a1f * a2f;
		aOut = toRepr<O>(aOutf);
	}
};

}  // namespace Impl

template <Tp::Repr::Flags R1, Tp::Repr::Flags R2, Tp::Repr::Flags O>
inline void mulA3(ReTp<R1> a1, ReTp<R2> a2, ReTp<O> &aOut)
{
	Impl::MulA3<R1 & Tp::Repr::MaskTraitScalar, R2 & Tp::Repr::MaskTraitScalar,
		O & Tp::Repr::MaskTraitScalar>::call(a1, a2, aOut);
}

// sum

namespace Impl {

template <Tp::Repr::Flags R1, Tp::Repr::Flags R2, Tp::Repr::Flags O>
struct SumA3 {
	static inline void call(ReTp<R1> a1, ReTp<R2> a2, ReTp<O> &aOut)
	{
		float a1f = fromRepr<float, R1>(a1);
		float a2f = fromRepr<float, R2>(a2);
		float aOutf = a1f + a2f;
		aOut = toRepr<O>(aOutf);
	}
};

}  // namespace Impl

template <Tp::Repr::Flags R1, Tp::Repr::Flags R2, Tp::Repr::Flags O>
inline void sumA3(ReTp<R1> a1, ReTp<R2> a2, ReTp<O> &aOut)
{
	Impl::SumA3<R1 & Tp::Repr::MaskTraitScalar, R2 & Tp::Repr::MaskTraitScalar,
		O & Tp::Repr::MaskTraitScalar>::call(a1, a2, aOut);
}

// abs

namespace Impl {

template <Tp::Repr::Flags F>
struct Abs {
	static inline void call(ReTp<F> &a1)
	{
		a1 = toRepr<F>(abs(fromRepr<float, F>(a1)));
	}
};

}  // namespace Impl

template <Tp::Repr::Flags F>
inline void abs(ReTp<F> &a)
{
	Impl::Abs<F & Tp::Repr::MaskTraitScalar>::call(a);
}

// gt

namespace Impl {

template <Tp::Repr::Flags F1, Tp::Repr::Flags F2>
struct Gt {
	static inline bool call(const ReTp<F1> &aLhs, const ReTp<F2> &aRhs)
	{
		return fromRepr<float, F1>(aLhs) > fromRepr<float, F2>(aRhs);
	}
};

template <Tp::Repr::Flags F>
struct Gt<F, F> {
	static inline bool call(const ReTp<F> &aLhs, const ReTp<F> &aRhs, En<F & Tp::Repr::ReprRaw> = nullptr)
	{
		return aLhs > aRhs;
	}
};

}  // namespace Impl

template <Tp::Repr::Flags F1, Tp::Repr::Flags F2 = F1>
inline bool gt(const ReTp<F1> &aLhs, const ReTp<F2>(aRhs))
{
	return Impl::Gt<F1 & Tp::Repr::MaskTraitScalar, F2 & Tp::Repr::MaskTraitScalar>::call(aLhs, aRhs);
}

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_ARITHM_HPP_
