//
// PrecHgMem.hpp
//
// Created on: Sep 09, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_MEM_PRECHGMEM_HPP_)
#define MOSSE_UTIL_MEM_PRECHGMEM_HPP_

#include "Util/Mem.hpp"

namespace Mosse {
namespace Ut {

/// \brief Provides precompiled Hann and Gauss matrices
///
class PrecHgMem : public Mem {
private:
	struct PrecompiledMatrices {
		const void *gauss;
		const void *hann;
	};
public:
	PrecHgMem(const void *kGauss, const void *kHann);
	const void *hannMatrix();
	const void *gaussKernel();
private:
	PrecompiledMatrices mat;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_MEM_PRECHGMEM_HPP_
