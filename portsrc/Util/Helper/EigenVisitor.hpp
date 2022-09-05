//
// EigenVisitor.hpp
//
// Created on: Aug 31, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_EIGENVISITOR_HPP_)
#define MOSSE_UTIL_EIGENVISITOR_HPP_

#include "Util/Helper/IndexSequence.hpp"
#include "Util/Helper/ReTp.hpp"
#include "Util/Helper/En.h"
#include "Util/Arithm/Conv.hpp"
#include "Util/Arithm/Arithm.hpp"
#include "Types/Tracking.hpp"
#include <utility>

namespace Mosse {
namespace Ut {

template <Tp::Repr::Flags R>
struct MaxVisitor {
	using ValueType = ReTp<R>;

	inline void init(const ValueType &aValueType, unsigned row, unsigned col)
	{
		max = aValueType;
		pos = {row, col};
	}

	inline void operator()(const ValueType &aValueType, unsigned row, unsigned col)
	{
		if (Ut::gt<R, R>(aValueType, max)) {
			max = aValueType;
			pos = {row, col};
		}
	}

	ValueType max;
	Tp::PointRowCol pos;
};

template <Tp::Repr::Flags F>
struct FloatSumVisitor {
	using ValueType = ReTp<F>;

	inline void init(const ValueType &, unsigned, unsigned)
	{
		sum = 0.0f;
	}

	inline void operator()(const ValueType &aValueType, unsigned row, unsigned col)
	{
		sum += fromRepr<float, F>(aValueType);
	}

	float sum;
};

/// \brief Calculates sum of absolute deviations from the mean value, taking mask into account
///
template <Tp::Repr::Flags F, bool Fmask = true>
struct FloatDevSumVisitor {
	using ValueType = ReTp<F>;
	inline void init(const ValueType &, unsigned, unsigned)
	{
		devsum = 0.0f;
	}

	void call(const ValueType &aVal)
	{
		devsum += fabs(Ut::fromRepr<float, F>(aVal) - mean);
	}

	template <bool C = Fmask>
	inline typename std::enable_if<C>::type operator()(const ValueType &aValueType, unsigned row, unsigned col)
	{
		call(aValueType);
	}

	template <bool C = Fmask>
	inline typename std::enable_if<!C>::type operator()(const ValueType &aValueType, unsigned row, unsigned col)
	{
		if (!mask.isInside({row, col})) {
			call(aValueType);
		}
	}

	float devsum;
	float mean;
	Tp::Roi mask;
};

template <Tp::Repr::Flags F, class ...Vs>
struct CompositeVisitor {
	using ValueType = ReTp<F>;

	static constexpr auto kN = sizeof...(Vs);
	std::tuple<Vs...> visitors;

	template <unsigned ...Is>
	inline void initAll(const ValueType &aValue, unsigned row, unsigned col, IndexSequence<Is...>)
	{
		using List = int[];
		(void)List{(void(std::get<Is>(visitors).init(aValue, row, col)), 0)...};
	}

	template <unsigned ...Is>
	inline void invokeAll(const ValueType &aValue, unsigned row, unsigned col, IndexSequence<Is...>)
	{
		using List = int[];
		(void)List{(void(std::get<Is>(visitors)(aValue, row, col)), 0)...};
	}

	inline void init(const ValueType &val, unsigned row, unsigned col)
	{
		initAll(val, row, col, makeIndexSequence<kN>());
	}

	inline void operator()(const ValueType &val, unsigned row, unsigned col)
	{
		invokeAll(val, row, col, makeIndexSequence<kN>());
	}

	template <std::size_t N>
	auto get() -> decltype(std::get<N>(visitors))
	{
		static_assert(N < kN, "");
		std::get<N>(visitors);
	}
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_EIGENVISITOR_HPP_
