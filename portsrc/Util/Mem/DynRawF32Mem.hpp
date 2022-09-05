//
// DynRawF32Mem.hpp
//
// Created on: Sep 05, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_MEM_DYNRAWF32MEM_HPP_)
#define MOSSE_UTIL_MEM_DYNRAWF32MEM_HPP_

#include "Util/Mem.hpp"
#include <vector>

namespace Mosse {
namespace Tp {

class PointRowCol;

}  // namespace Tp

namespace Ut {

class DynRawF32Mem : public Mem {
public:
	DynRawF32Mem();
	virtual void *matA() override;
	virtual void *matB() override;
	virtual void *buffer() override;
	virtual void init(Tp::Roi) override;
private:
	std::vector<float> mata;
	std::vector<float> matb;
	std::vector<float> buf;
	Tp::PointRowCol roiSizePrev;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_MEM_DYNRAWF32MEM_HPP_
