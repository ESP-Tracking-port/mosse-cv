//
// Port.hpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_PORT_HPP_)
#define MOSSE_UTIL_PORT_HPP_

namespace Mosse {
namespace Ut {

class Ops;
class Mem;

/// \brief Replaceable operations
///
struct Port {
	Ops &ops;
	Mem &mem;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_PORT_HPP_
