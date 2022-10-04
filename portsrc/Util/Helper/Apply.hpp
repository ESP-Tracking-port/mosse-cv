//
// Apply.hpp
//
// Created on: Sep 19, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_HELPER_APPLY_HPP_)
#define MOSSE_UTIL_HELPER_APPLY_HPP_

#include "portsrc/Util/Helper/IndexSequence.hpp"
#include <tuple>
#include <type_traits>

namespace Mosse {
namespace Ut {
namespace Impl {

template <class C, class T, unsigned ...Is>
auto applyImpl(C &&aCallable, T &&aTuple, IndexSequence<Is...>) -> decltype(aCallable(std::get<Is>(aTuple)...))
{
	return aCallable(std::get<Is>(aTuple)...);
}

}  // namespace Impl

template <class C, class T>
inline auto apply(C &&aCallable, T &&aTuple) -> decltype(Impl::applyImpl(std::forward<C>(aCallable),
	std::forward<T>(aTuple), makeIndexSequence<std::tuple_size<typename std::remove_reference<T>::type>::value>()))
{
	return Impl::applyImpl(std::forward<C>(aCallable), std::forward<T>(aTuple),
		makeIndexSequence<std::tuple_size<typename std::remove_reference<T>::type>::value>());
}

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_HELPER_APPLY_HPP_
