//
// StoreType.hpp
//
// Created on: Aug 31, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_STORETYPE_HPP_)
#define MOSSE_UTIL_STORETYPE_HPP_

namespace Mosse {
namespace Ut {

template <class T>
struct StoreType {
	using Type = T;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_STORETYPE_HPP_
