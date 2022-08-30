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

Tracker::Tracker(Ut::Port aPort, float aEta) : tracking{{}, 0.0f, aEta}, port{aPort}
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

	port.mem.init(tracking.roi);
	port.ops.init(tracking.roi);
	port.ops.imageCropInto(aImage, port.mem.buffer());
	port.ops.imagePreprocess(port.mem.buffer());
	port.ops.fft2(port.mem.buffer());
	port.ops.mataUpdate(port.mem.matA(), tracking.eta, true);
	port.ops.matbUpdate(port.mem.matB(), port.mem.buffer(), tracking.eta, true);
}

void Tracker::update(Tp::Image aImage, bool aUpdatePsr)
{
	// Calculate convolution in the frequency domain
	port.ops.imageCropInto(aImage, port.mem.buffer());
	port.ops.imagePreprocess(port.mem.buffer());
	port.ops.fft2(port.mem.buffer());
	port.ops.imageConvFftDomain(port.mem.buffer(), port.mem.matA(), port.mem.matB());
	port.ops.ifft2(port.mem.buffer());  // Now buffer stores response in time domain
	Tp::PointRowCol maxResponsePos{};  // TODO: init

	if (aUpdatePsr) {
		float sum = 0.0f;
		port.ops.maxReal(port.mem.buffer(), maxResponsePos, sum);
		const Tp::PointRowCol kMaskSize{11, 11};
		tracking.psr = port.ops.calcPsr(port.mem.buffer(), maxResponsePos, sum, kMaskSize);
	} else {
		port.ops.maxReal(port.mem.buffer(), maxResponsePos, nullptr);
	}

	// Update ROI
	tracking.roi = {tracking.roi.origin + maxResponsePos - tracking.roi.size / 2, tracking.roi.size};
	port.ops.init(tracking.roi);
	port.mem.init(tracking.roi);
	// Retrain w/ the new ROI
	port.ops.imageCropInto(aImage, port.mem.buffer());
	port.ops.imagePreprocess(port.mem.buffer());
	port.ops.mataUpdate(port.mem.matA(), tracking.eta, false);
	port.ops.fft2(port.mem.buffer());
	port.ops.matbUpdate(port.mem.matB(), port.mem.buffer(), tracking.eta, false);
}

float Tracker::psr()
{
	return tracking.psr;
}

const Tp::Roi &Tracker::roi() const
{
	return tracking.roi;
}

float Tracker::lastPsr() const
{
	return tracking.psr;
}

}  // namespace Mosse
