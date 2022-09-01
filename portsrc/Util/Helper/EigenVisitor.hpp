//
// EigenVisitor.hpp
//
// Created on: Aug 31, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_EIGENVISITOR_HPP_)
#define MOSSE_UTIL_EIGENVISITOR_HPP_

#include "Util/Helper/IndexSequence.hpp"
#include "Util/Arithm/Arithm.hpp"
#include "Types/Tracking.hpp"
#include <utility>

namespace Mosse {
namespace Ut {

template <class T, Tp::Repr::Flags R>
struct MaxVisitor {
	inline void init(const T &aValueType, unsigned row, unsigned col)
	{
		max = aValueType;
		pos = {row, col};
	}

	inline void operator()(const T &aValueType, unsigned row, unsigned col)
	{
		if (max < aValueType) {
			max = aValueType;
			pos = {row, col};
		}
	}

	T max;
	Tp::PointRowCol pos;
};

template <class ValueType, Tp::Repr::Flags F>
struct FloatSumVisitor {
	inline void init(const ValueType &, unsigned, unsigned)
	{
		sum = 0.0f;
	}

	inline void operator()(const ValueType &aValueType, unsigned row, unsigned col)
	{
		sum += fromRepr<float, F>(&aValueType);
	}

	float sum;
};

template <class T, class ...Vs>
struct CompositeVisitor {
	static constexpr auto kN = sizeof...(Vs);
	std::tuple<Vs...> visitors;

	template <std::size_t ...Is>
	inline void initAll(const T &aValue, unsigned row, unsigned col, IndexSequence<Is...>)
	{
		using List = int[];
		(void)List{(void(std::get<Is>(visitors).init(aValue, row, col)), 0)...};
	}

	template <std::size_t ...Is>
	inline void invokeAll(const T &aValue, unsigned row, unsigned col, IndexSequence<Is...>)
	{
		using List = int[];
		(void)List{(void(std::get<Is>(visitors)(aValue, row, col)), 0)...};
	}

	inline void init(const T &val, unsigned row, unsigned col)
	{
		initAll(val, row, col, makeIndexSequence<kN>());
	}

	inline void operator()(const T &val, unsigned row, unsigned col)
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