//
// Apply.hpp
//
// Created on: Sep 19, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_HELPER_APPLY_HPP_)
#define MOSSE_UTIL_HELPER_APPLY_HPP_

#include "Util/Helper/IndexSequence.hpp"
#include <tuple>

namespace Mosse {
namespace Ut {
namespace Impl {

template <class C, class T, unsigned ...Is>
auto apply(C &&aCallable, T &&aTuple, IndexSequence<Is...>) -> decltype(aCallable(std::get<Is>(aTuple)...))
{
	return aCallable(std::get<Is>(aTuple)...);
}

}  // namespace Impl

template <class C, class T>
inline auto apply(C &&aCallable, T &&aTuple) -> decltype(Impl::apply(std::forward<C>(aCallable),
	std::forward<T>(aTuple)), makeIndexSequence<std::tuple_size<T>::value>())
{
	return Impl::apply(std::forward<C>(aCallable), std::forward<T>(aTuple)),
		makeIndexSequence<std::tuple_size<T>::value>();
}

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_HELPER_APPLY_HPP_
