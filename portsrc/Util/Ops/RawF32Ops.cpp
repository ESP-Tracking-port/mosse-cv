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
	static constexpr auto gaussKernelScaledGet = Mosse::getGaussKernelFft3dScaled125;
	static constexpr auto kEta = 0.125f;

	if (roiSizePrev != roi().size) {
		roiSizePrev = roi().size;
		precompiledMatrices = {
			Mosse::getHann(roi().size(0), roi().size(1)),
			gaussKernelScaledGet(roi().size(0), roi().size(1))
		};
		setEta(kEta);
	}
}

const void *RawF32Ops::hannMatrix()
{
	return precompiledMatrices.hann;
}

const void *RawF32Ops::gaussFft()
{
	return precompiledMatrices.gaussFftScaled;
}

}  // namespace Ut
}  // namespace Mosse
