//
// Ops.cpp
//
// Created on: Aug 31, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Types/Tracking.hpp"
#include "Util/Arithm/Conv.hpp"
#include "Ops.hpp"
#include "MosseApi.hpp"
#include <OhDebug.hpp>

namespace Mosse {
namespace Ut {

void Ops::init(Tp::Roi aRoi)
{
	mRoi = aRoi;
	ohdebug(Ops::init, aRoi.size(0), mRoi.size(0));
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
	aRoi.readjust({rows, cols});
}

}  // namespace Ut
}  // namespace Mosse
