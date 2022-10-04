//
// Tracker.cpp
//
// Created on: Aug 29, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "portsrc/Port/MossePort.hpp"
#include "portsrc/Types/Port.hpp"
#include "portsrc/Types/Tracking.hpp"
#include "portsrc/Types/Repr.hpp"
#include "portsrc/Util/Mem.hpp"
#include "portsrc/Util/Ops.hpp"
#include "Tracker.hpp"
#include "portsrc/Port/MossePort.hpp"

namespace Mosse {

Tracker::Tracker(Ut::Port aPort) : tracking{{}, 0.0f}, port{aPort}
{
}

void Tracker::init(Mosse::Tp::Image aImage, Mosse::Tp::Roi aRoi)
{
	MallocCounter mc{};
	(void)mc;
	// A set of precompiled gaussian matrices is used, so the window's size will be changed to the closest one
	tracking.roi = aRoi;
	port.ops.roiResize(tracking.roi);
	tracking.roi.fitShift({aImage.rows(), aImage.cols()});
	port.mem.init(tracking.roi);
	mosse_assert(nullptr != port.mem.buffer());
	mosse_assert(nullptr != port.mem.matA());
	mosse_assert(nullptr != port.mem.matB());
	port.ops.tearDown();  // Abrupt previous tracking flow
	ohdebugstr(Tracker::init, port.ops.init(tracking.roi));
	port.ops.imageCropInto(aImage, port.mem.buffer());
	port.ops.imagePreprocess(port.mem.buffer());
	port.ops.fft2(port.mem.buffer());
	port.ops.mataUpdate(port.mem.matA(), port.mem.buffer(), true);
	port.ops.matbUpdate(port.mem.matB(), port.mem.buffer(), true);

	// TODO: rand warp-based pretraining XXX: It seems to work without
}

void Tracker::update(Tp::Image aImage, bool aUpdatePsr)
{
	MallocCounter mc{};
	(void)mc;
	// Calculate convolution in the frequency domain
	port.ops.imageCropInto(aImage, port.mem.buffer());
	port.ops.imagePreprocess(port.mem.buffer());
	port.ops.fft2(port.mem.buffer());
	port.ops.imageConvFftDomain(port.mem.buffer(), port.mem.matA(), port.mem.matB());
	// Back to time domain
	port.ops.ifft2(port.mem.buffer());  // Now buffer stores response in time domain
	// TODO: XXX: The reference implementation used normalization (probably, to prevent overflows).
	Tp::PointRowCol maxResponsePos{};

	if (aUpdatePsr) {
		float sum = 0.0f;
		port.ops.maxReal(port.mem.buffer(), maxResponsePos, &sum);
		const Tp::PointRowCol kMaskSize{11, 11};
		tracking.psr = port.ops.calcPsr(port.mem.buffer(), maxResponsePos, sum, kMaskSize);
	} else {
		port.ops.maxReal(port.mem.buffer(), maxResponsePos, nullptr);
	}

	// Update ROI
	tracking.roi.setCenter(maxResponsePos + roi().origin);  // Shift the window
	tracking.roi.fitShift({aImage.rows(), aImage.cols()});  // Make sure the window fits the image. Adjust its coordinates while preserving the size
	port.ops.init(tracking.roi);
	port.mem.init(tracking.roi);
	// Retrain w/ the new ROI
	port.ops.imageCropInto(aImage, port.mem.buffer());
	port.ops.imagePreprocess(port.mem.buffer());
	port.ops.fft2(port.mem.buffer());
	port.ops.mataUpdate(port.mem.matA(), port.mem.buffer(), false);
	port.ops.matbUpdate(port.mem.matB(), port.mem.buffer(), false);
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
