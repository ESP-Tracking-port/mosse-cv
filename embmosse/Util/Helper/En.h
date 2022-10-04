//
// En.h
//
// Created on: Sep 01, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//
// Shortcut version of std::enable_if

#if !defined(MOSSE_UTIL_HELPER_EN_HPP_)
#define MOSSE_UTIL_HELPER_EN_HPP_

#include <type_traits>

namespace Mosse {
namespace Ut {

template <bool C>
using En = typename std::enable_if<C>::type *;

}  // namespace Ut

namespace Tp {

template <bool C>
using En = typename std::enable_if<C>::type *;

}  // namespace Tp
}  // namespace Mosse

#endif // MOSSE_UTIL_HELPER_EN_HPP_
