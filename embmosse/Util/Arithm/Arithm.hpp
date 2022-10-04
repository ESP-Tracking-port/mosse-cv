//
// Arithm.hpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_ARITHM_HPP_)
#define MOSSE_UTIL_ARITHM_HPP_

#include "embmosse/Types/Repr.hpp"
#include "embmosse/Util/Helper/En.h"
#include "embmosse/Util/Helper/ReTp.hpp"
#include "embmosse/Util/Arithm/Conv.hpp"
#include "embmosse/Util/Helper/FpmHelper.hpp"
#include "embmosse/Port/MossePort.hpp"

namespace Mosse {
namespace Ut {

template <Tp::Repr::Flags R1, Tp::Repr::Flags R2, Tp::Repr::Flags O>
void mulCplxA3(ReTp<R1> aRe1, ReTp<R1> aIm1, ReTp<R2> aRe2, ReTp<R2> aIm2, ReTp<O> &aoRe, ReTp<O> &aoIm);

namespace Impl {

// Complex multiplication

/// \brief Default implementation, just converts from representation to floats, and performs multiplication.
/// Optimizations will be achieved through template specialization.
///
template <Tp::Repr::Flags R1, Tp::Repr::Flags R2, Tp::Repr::Flags O>
struct MulCplxA3 {
	static inline void call(ReTp<R1> aRe1, ReTp<R1> aIm1, ReTp<R2> aRe2, ReTp<R2> aIm2, ReTp<O> &aoRe, ReTp<O> &aoIm)
	{
		auto a = fromRepr<float, R1>(aRe1);
		auto b = fromRepr<float, R1>(aIm1);
		auto c = fromRepr<float, R2>(aRe2);
		auto d = fromRepr<float, R2>(aIm2);
		auto ac = a * c;
		auto bd = b * d;
		aoRe = toRepr<O>(ac - bd);
		aoIm = toRepr<O>((a + b) * (c + d) - ac - bd);
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
		static constexpr float kEpsilon = 0.0001f;
		float oRef = (a * c + b * d) / (c * c + d * d + kEpsilon);
		float oImf = (b * c - a * d) / (c * c + d * d + kEpsilon);
		aoRe = toRepr<O>(oRef);
		aoIm = toRepr<O>(oImf);
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

template <>
struct MulA3<Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint, Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint,
	Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint>
{
	static constexpr Tp::Repr::Flags kI16 = Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint;

	static inline void call(ReTp<kI16> a1, ReTp<kI16> a2, ReTp<kI16> &aOut)
	{
		aOut = (makeFpmFixedFromRaw<kI16>(a1) * makeFpmFixedFromRaw<kI16>(a2)).raw_value();
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

template <>
struct SumA3<Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint, Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint,
	Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint> {
	static constexpr Tp::Repr::Flags kI16 = Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint;
	static inline void call(ReTp<kI16> a1, ReTp<kI16> a2, ReTp<kI16> &aOut)
	{
		aOut = (makeFpmFixedFromRaw<kI16>(a1) + makeFpmFixedFromRaw<kI16>(a2)).raw_value();
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
	static inline bool call(ReTp<F1> aLhs, ReTp<F2> aRhs)
	{
		return fromRepr<float, F1>(aLhs) > fromRepr<float, F2>(aRhs);
	}
};

template <Tp::Repr::Flags F>
struct Gt<F, F> {
	static inline bool call(ReTp<F> aLhs, ReTp<F> aRhs, En<F & Tp::Repr::ReprRaw> = nullptr)
	{
		return aLhs > aRhs;
	}
};

}  // namespace Impl

template <Tp::Repr::Flags F1, Tp::Repr::Flags F2 = F1>
inline bool gt(ReTp<F1> aLhs, ReTp<F2>(aRhs))
{
	return Impl::Gt<F1 & Tp::Repr::MaskTraitScalar, F2 & Tp::Repr::MaskTraitScalar>::call(aLhs, aRhs);
}

/// \brief Convenient wrapper over arithmetic operaitons. Removes the necessity to use templates in modules where
/// there are only a few arithmetic operations required.
///
struct ArithmBase {
	virtual bool gt(const Tp::NumVariant aLhs, const Tp::NumVariant aRhs) const = 0;
	virtual void sumA3(const Tp::NumVariant aLhs, const Tp::NumVariant aRhs, Tp::NumVariant &aOut) const = 0;
};

template <Tp::Repr::Flags F, Tp::Repr::Flags F2 = F, Tp::Repr::Flags F3 = F>
struct Arithm : ArithmBase {
	bool gt(const Tp::NumVariant aLhs, const Tp::NumVariant aRhs) const override
	{
		return Ut::gt<F>(aLhs, aRhs);
	}

	void sumA3(const Tp::NumVariant aLhs, const Tp::NumVariant aRhs, Tp::NumVariant &aOut) const override
	{
		ReTp<F> res;
		Ut::sumA3<F, F, F>(aLhs, aRhs, res);
		aOut = {res};
	}
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_ARITHM_HPP_
