//
// EigenSeq.cpp
//
// Created on: Sep 14, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Types/Tracking.hpp"
#include "Util/Arithm/Algo.hpp"
#include "EigenSeq.hpp"

namespace Mosse {
namespace Ut {

DiagonalRowsSeq::DiagonalRowsSeq(const Mosse::Tp::PointRowCol &aSize) : sz{static_cast<std::size_t>(aSize(0))}
{
}

DiagonalColsSeq::DiagonalColsSeq(const Tp::PointRowCol &aSize, bool aMainDiagonal) : sz{aSize}, main{aMainDiagonal}
{
}

std::size_t DiagonalColsSeq::operator[](std::size_t index) const
{
	std::size_t ret = 0;

	if (main) {
		ret = index * sz(0) / sz(0);
	} else {
		ret = sz(1) - index * sz(1) / sz(0);
	}

	Ut::clamp<std::size_t>(ret, 0, sz(1));

	return ret;
}

}  // namespace Ut
}  // namespace Mosse
