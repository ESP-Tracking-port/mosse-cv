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

}  // namespace Tp

namespace Ut {

class Mem {
public:
	virtual void *matA() = 0;
	virtual void *matB() = 0;
	virtual void *buffer() = 0;  ///< A big intermediary buffer for operations like FFT
	virtual ~Mem() = default;

	/// \brief Implementation MUST ensure that it is able to provide a sufficient buffer taking its internal numeric
	/// representation into account. Implementation MUST expect, that it will be called multiple times and therefore
	/// optimize for unnecessary time and memory expenses (e.g. to not to reallocate memory, if the new ROI's size does
	/// not exceed that of the previous one;
	///
	virtual void *init(Tp::Roi);
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_MEM_HPP_
