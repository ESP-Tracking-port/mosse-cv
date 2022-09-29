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
#include "Util/Helper/FpmHelper.hpp"
#include "Port/MossePort.hpp"
#include <cassert>

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

template <>
struct MulCplxA3<Tp::Repr::StorageF32 | Tp::Repr::ReprRaw, Tp::Repr::StorageF32 | Tp::Repr::ReprRaw,
	Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint>
{
	static constexpr Tp::Repr::Flags kI16 = Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint;
	static constexpr Tp::Repr::Flags kF32 = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw;

	static inline void call(ReTp<kF32> aRe1, ReTp<kF32> aIm1, ReTp<kF32> aRe2, ReTp<kF32> aIm2, ReTp<kI16> &aoRe,
		ReTp<kI16> &aoIm)
	{
		float re;
		float im;
		mulCplxA3<kF32, kF32, kF32>(aRe1, aIm1, aRe2, aIm2, re, im);
		aoRe = Ut::makeFpmFixed<kI16>(re).raw_value();
		aoIm = Ut::makeFpmFixed<kI16>(im).raw_value();
	}
};

template <>
struct MulCplxA3<Tp::Repr::StorageF32 | Tp::Repr::ReprRaw, Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint,
	Tp::Repr::StorageF32 | Tp::Repr::ReprRaw>
{
	static constexpr Tp::Repr::Flags kI16 = Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint;
	static constexpr Tp::Repr::Flags kF32 = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw;

	static inline void call(ReTp<kF32> aRe1, ReTp<kF32> aIm1, ReTp<kI16> aRe2, ReTp<kI16> aIm2, ReTp<kF32> &aoRe,
		ReTp<kF32> &aoIm)
	{
		auto re2 = static_cast<ReTp<kF32>>(makeFpmFixedFromRaw<kI16>(aRe2));
		auto im2 = static_cast<ReTp<kF32>>(makeFpmFixedFromRaw<kI16>(aIm2));
		Ut::mulCplxA3<kF32, kF32, kF32>(aRe1, aIm1, re2, im2, aoRe, aoIm);
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
	}
};

template <>
struct DivCplxA3<Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint, Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint,
	Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint>
{
	static constexpr Tp::Repr::Flags kI16 = Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint;

	static inline void call(ReTp<kI16> aRe1, ReTp<kI16> aIm1, ReTp<kI16> aRe2, ReTp<kI16> aIm2, ReTp<kI16> &aoRe,
		ReTp<kI16> &aoIm)
	{
		auto a = makeFpmFixedFromRaw<kI16>(aRe1);
		auto b = makeFpmFixedFromRaw<kI16>(aIm2);
		auto c = makeFpmFixedFromRaw<kI16>(aRe2);
		auto d = makeFpmFixedFromRaw<kI16>(aIm2);
		aoRe = ((a * c + b * d) / (c * c + d * d)).raw_value();
		aoIm = ((b * c - a * d) / (c * c + d * d)).raw_value();
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
	static constexpr Tp::Repr::Flags kI16 = Tp::Repr::ReprFixedPoint | Tp::Repr::StorageI16;

	static inline void call(ReTp<kI16> a1, ReTp<kI16> a2, ReTp<kI16> &ao)
	{
		ao = (makeFpmFixedFromRaw<kI16>(a1) * makeFpmFixedFromRaw<kI16>(a2)).raw_value();
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
	Tp::Repr::StorageI16 | Tp::Repr::ReprFixedPoint>
{
	static constexpr Tp::Repr::Flags kI16 = Tp::Repr::ReprFixedPoint | Tp::Repr::StorageI16;

	static inline void call(ReTp<kI16> a1, ReTp<kI16> a2, ReTp<kI16> &ao)
	{
		ao = (makeFpmFixedFromRaw<kI16>(a1) + makeFpmFixedFromRaw<kI16>(a2)).raw_value();
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
