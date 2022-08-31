//
// EigenVisitor.hpp
//
// Created on: Aug 31, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_EIGENVISITOR_HPP_)
#define MOSSE_UTIL_EIGENVISITOR_HPP_

#include "Util/IndexSequence.hpp"
#include "Types/Tracking.hpp"
#include <utility>

namespace Mosse {
namespace Ut {

template <class T>
struct MaxVisitor {
	void init(const T &aValueType, unsigned row, unsigned col)
	{
		max = aValueType;
		pos = {row, col};
	}

	void operator()(const T &aValueType, unsigned row, unsigned col)
	{
		if (max < aValueType) {
			max = aValueType;
			pos = {row, col};
		}
	}

	T max;
	Tp::PointRowCol pos;
};

template <class ValueType>
struct FloatSumVisitor {
	void init(const ValueType &, unsigned, unsigned)
	{
		sum = 0.0f;
	}

	void operator()(const ValueType &aValueType, unsigned row, unsigned col)
	{
		sum += static_cast<float>(aValueType);
	}

	float sum;
};

template <class T, class ...Vs>
struct CompositeVisitor {
	static constexpr auto kN = sizeof...(Vs);
	std::tuple<Vs...> visitors;

	template <std::size_t ...Is>
	void initAll(const T &aValue, unsigned row, unsigned col, IndexSequence<Is...>)
	{
		using List = int[];
		(void)List{(void(std::get<Is>(visitors).init(aValue, row, col)), 0)...};
	}

	template <std::size_t ...Is>
	void invokeAll(const T &aValue, unsigned row, unsigned col, IndexSequence<Is...>)
	{
		using List = int[];
		(void)List{(void(std::get<Is>(visitors)(aValue, row, col)), 0)...};
	}

	void init(const T &val, unsigned row, unsigned col)
	{
		initAll(val, row, col, makeIndexSequence<kN>());
	}

	void init(const T &val, unsigned row, unsigned col)
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
