//
// Mem.hpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_TYPES_MEM_HPP_)
#define MOSSE_TYPES_MEM_HPP_

#include <cstdint>

namespace Mosse {
namespace Tp {

static constexpr inline std::uint64_t bit()
{
	return 0;
}

template <class T, class ...Ts>
static constexpr inline std::uint64_t bit(T t, Ts ...aTs)
{
	using List = std::uint64_t[];

	return 1 << t | bit(aTs...);
}

struct Repr {
	enum Flag : std::uint64_t {
		LenU16 = bit(0),
		LenU32 = bit(1),
		MaskLen = bit(0, 1),
		ReprDirect = bit(2),
		ReprLog2 = bit(3),
		MaskRepr = bit(2, 3),
	};
};

class Geometry {
	std::size_t elementSizeof;
};

}  // namespace Tp
}  // namespace Mosse

#endif // MOSSE_TYPES_MEM_HPP_
