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
	if (roiSizePrev != roi().size) {
		roiSizePrev = roi().size;
		//TODO: The rest of the initialization routine
	}
}

}  // namespace Ut
}  // namespace Mosse
