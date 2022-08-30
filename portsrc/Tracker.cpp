//
// Tracker.cpp
//
// Created on: Aug 29, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Types/Port.hpp"
#include "Types/Tracking.hpp"
#include "Types/Mem.hpp"
#include "Util/Fft.hpp"
#include "Util/Mem.hpp"
#include "Util/Ops.hpp"
#include "Util/Fft.hpp"
#include "Tracker.hpp"
#include "MosseApi.hpp"

namespace Mosse {

Tracker::Tracker(Ut::Port aPort) : port{aPort}
{
}

void Tracker::init(Mosse::Tp::Image aImage, Mosse::Tp::Roi aRoi)
{
	// A set of precompiled gaussian matrices is used, so the window's size will be changed to the closest one
	{
		unsigned rows = aRoi.size(0);
		unsigned cols = aRoi.size(1);
		Mosse::getClosestWindow(rows, cols);
		tracking.roi.origin = {aRoi.origin(0) + aRoi.size(0) / 2 - rows / 2,
			aRoi.origin(1) + aRoi.size(1) / 2 - cols / 2};
		tracking.roi.size = {rows, cols};
	}
}

float Tracker::psr()
{
	return tracking.psr;
}

const Tp::Roi &Tracker::roi() const
{
	return tracking.roi;
}

}  // namespace Mosse
