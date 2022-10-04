//
// DynRawMem.hpp
//
// Created on: Sep 29, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_MEM_DYNRAWMEM_HPP_)
#define MOSSE_UTIL_MEM_DYNRAWMEM_HPP_

#include "embmosse/Types/Repr.hpp"
#include "embmosse/Types/Tracking.hpp"
#include "embmosse/Util/Helper/ReTp.hpp"
#include "embmosse/Util/Mem.hpp"
#include <memory>
#include <type_traits>

namespace Mosse {
namespace Ut {

/// \tparam RoiSz if 0, the allocated size will be determined in-situ using the ROI's area.
///
template <Tp::Repr::Flags ReprBuf, Tp::Repr::Flags ReprAb, std::size_t RoiSz = 0>
class DynRawMem : public Mem {
public:
	DynRawMem() : Mem(), roiPrev{{0, 0}, {0, 0}}
	{
	}
	void *matA() override
	{
		return ptrMatA.get();
	}
	void *matB() override
	{
		return ptrMatB.get();
	}
	void *buffer() override
	{
		return ptrBuf.get();
	}
	void init(Tp::Roi aRoi) override
	{
		initImpl(aRoi);
	}
private:
	template <std::size_t S = RoiSz>
	typename std::enable_if<(S > 0)>::type initImpl(const Tp::Roi &aRoi)
	{
		mosse_assert(aRoi.area() <= S);

		if (!static_cast<bool>(ptrBuf)) {
			ptrBuf = {new ReTp<ReprBuf>[S * 2]};
			ptrMatA = {new ReTp<ReprAb>[S * 2]};
			ptrMatB = {new ReTp<ReprAb>[S * 2]};
		}
	}
	template <std::size_t S = RoiSz>
	typename std::enable_if<(S == 0)>::type initImpl(const Tp::Roi &aRoi)
	{
		if (aRoi.area() > roiPrev.area()) {
			roiPrev = aRoi;
			ptrBuf = std::unique_ptr<ReTp<ReprBuf>[]>{new ReTp<ReprBuf>[aRoi.area() * 2]};
			ptrMatA = std::unique_ptr<ReTp<ReprAb>[]>{new ReTp<ReprAb>[aRoi.area() * 2]};
			ptrMatB = std::unique_ptr<ReTp<ReprAb>[]>{new ReTp<ReprAb>[aRoi.area() * 2]};
		}
	}
private:
	std::unique_ptr<ReTp<ReprBuf>[]> ptrBuf;
	std::unique_ptr<ReTp<ReprAb>[]> ptrMatA;
	std::unique_ptr<ReTp<ReprAb>[]> ptrMatB;
	Tp::Roi roiPrev;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_MEM_DYNRAWMEM_HPP_
