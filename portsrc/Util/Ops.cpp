//
// Ops.cpp
//
// Created on: Aug 31, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Types/Tracking.hpp"
#include "Util/Arithm/Conv.hpp"
#include "Util/MosseDebug.hpp"
#include "Ops.hpp"
#include "MosseApi.hpp"

namespace Mosse {
namespace Ut {

void Ops::init(Tp::Roi aRoi)
{
	mRoi = aRoi;
	ohdebug(Ops::init, aRoi);
}

const Tp::Roi &Ops::roi() const
{
	return mRoi;
}

void Ops::initImpl()
{
}

void Ops::roiResize(Mosse::Tp::Roi &aRoi)
{
	unsigned rows = aRoi.size(0);
	unsigned cols = aRoi.size(1);
	Mosse::getClosestWindow(rows, cols);
	ohdebug(Ops::roiResize, "closest window", rows, cols);
	aRoi.readjust({rows, cols});
}

}  // namespace Ut
}  // namespace Mosse
