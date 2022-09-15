//
// Tracker.cpp
//
// Created on: Aug 29, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "MossePort.hpp"
#include "Types/Port.hpp"
#include "Types/Tracking.hpp"
#include "Types/Repr.hpp"
#include "Util/Mem.hpp"
#include "Util/Ops.hpp"
#include "Tracker.hpp"
#include "MossePort.hpp"

namespace Mosse {

Tracker::Tracker(Ut::Port aPort) : tracking{{}, 0.0f}, port{aPort}
{
}

void Tracker::init(Mosse::Tp::Image aImage, Mosse::Tp::Roi aRoi)
{
	ohdebugonce(Tracker::init, 0, aImage.rows(), aImage.cols(), aRoi);
	// A set of precompiled gaussian matrices is used, so the window's size will be changed to the closest one
	tracking.roi = aRoi;
	port.ops.roiResize(tracking.roi);
	tracking.roi.fitShift({aImage.rows(), aImage.cols()});
	ohdebugonce(Tracker::init, 0, "ROI after crop", tracking.roi);
	port.mem.init(tracking.roi);
	ohdebugstr(Tracker::init, port.ops.init(tracking.roi));
	port.ops.imageCropInto(aImage, port.mem.buffer());
	port.ops.imagePreprocess(port.mem.buffer());
	ohdebug(Tracker::init, "fft2");
	port.ops.fft2(port.mem.buffer());
	ohdebug(Tracker::init, "mataUpdate");
	port.ops.mataUpdate(port.mem.matA(), port.mem.buffer(), true);
	ohdebug(Tracker::init, port.mem.matA() != nullptr);
	ohdebug(Tracker::init, port.mem.matB() != nullptr);
	ohdebug(Tracker::init, "matbUpdate");
	port.ops.matbUpdate(port.mem.matB(), port.mem.buffer(), true);

	// TODO: rand warp-based pretraining XXX: It seems to work without
}

void Tracker::update(Tp::Image aImage, bool aUpdatePsr)
{
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
		ohdebug(Tracker::update, maxResponsePos);
	} else {
		port.ops.maxReal(port.mem.buffer(), maxResponsePos, nullptr);
		ohdebug(Tracker::update, maxResponsePos);
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
