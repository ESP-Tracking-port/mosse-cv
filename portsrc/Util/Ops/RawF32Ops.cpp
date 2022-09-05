//
// RawF32Ops.cpp
//
// Created on: Sep 05, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "MosseApi.hpp"
#include "RawF32Ops.hpp"

namespace Mosse {
namespace Ut {

void RawF32Ops::initImpl()
{
	if (roiPrev != roi()) {
		roiPrev = roi();
	}
}

}  // namespace Ut
}  // namespace Mosse
