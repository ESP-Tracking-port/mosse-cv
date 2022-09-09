//
// PrecHgMem.cpp
//
// Created on: Sep 09, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "PrecHgMem.hpp"

namespace Mosse {
namespace Ut {

PrecHgMem::PrecHgMem(const void *kGauss, const void *kHann) : mat{kGauss, kHann}
{
}

const void *PrecHgMem::hannMatrix()
{
	return mat.hann;
}

const void *PrecHgMem::gaussKernel()
{
	return mat.gauss;
}

}  // namespace Ut
}  // namespace Mosse
