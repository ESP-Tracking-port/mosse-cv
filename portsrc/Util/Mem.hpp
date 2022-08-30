//
// Mem.hpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_MEM_HPP_)
#define MOSSE_UTIL_MEM_HPP_

#include <cstdint>

namespace Mosse {
namespace Tp {

class Roi;
class Geometry;

}  // namespace Tp
namespace Ut {

class Mem {
public:
	Mem(Tp::Geometry);
	virtual void *matA() = 0;
	virtual void *matB() = 0;
	virtual void *buffer() = 0;  ///< A big intermediary buffer for operations like FFT
	const Tp::Geometry &geometry() const;
	virtual void *init(Tp::Roi) = 0;
private:
	Tp::Geometry geo;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_MEM_HPP_
