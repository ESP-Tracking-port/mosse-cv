//
// Tracker.cpp
//
// Created on: Aug 29, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "embmosse/Port/MossePort.hpp"
#include "embmosse/Types/Port.hpp"
#include "embmosse/Types/Tracking.hpp"
#include "embmosse/Types/Repr.hpp"
#include "embmosse/Util/Mem.hpp"
#include "embmosse/Util/Ops.hpp"
#include "Tracker.hpp"
#include "embmosse/Port/MossePort.hpp"

namespace Mosse {

Tracker::Tracker(Ut::Port aPort) : tracking{{}, 0.0f}, port{aPort}
{
}

/// \brief Creates a memory-safe working area by cropping a chunk of image into a separate memory location.
///
/// \details The duplication enables parallelization. If a frame is being formed by a separate thread (deployed on a
/// physical core), the frame-forming thread will be able to continue reusing the frame buffer without waiting for
/// the tracker to get finished with the frame. \sa `Mosse::Tracking::OffsetImage`
///
/// \arg aImage Mapped buffer
/// \arg aRoi Region of Interest.
///
/// \post If the tracker uses windows from a set of predefined shapes (sizes), `aRoi` will be resized to the closest
/// shape.
///
/// \pre ROI must be initialized
///
Tp::OffsetImage Tracker::imageCropWorkingArea(const Tp::Image &aImage)
{
	Tp::OffsetImage stub{tracking.roi, const_cast<std::uint8_t *>(aImage.data())};
#if MOSSE_MEM_CLONE_IMAGE_WORKING_AREA
	auto roi = tracking.roi;

	if (roi.area() == 0) {
		return stub;
	}

	roi.origin(0) -= roi.size(0) / 2;
	roi.origin(1) -= roi.size(1) / 2;
	roi.size(0) += roi.size(0);
	roi.size(1) += roi.size(1);
	roi.fitShift({aImage.rows(), aImage.cols()});
	port.mem.initImageWorkingArea(aImage, roi);
	mosse_assert(port.mem.imageWorkingArea() != nullptr);
	Tp::OffsetImage offsetImage{roi, static_cast<std::uint8_t *>(port.mem.imageWorkingArea())};

	return offsetImage;
#else
	return stub;
#endif
}

void Tracker::init(const Mosse::Tp::Image &aImage, Mosse::Tp::Roi aRoi)
{
	MallocCounter mc{};
	(void)mc;
	// A set of precompiled gaussian matrices is used, so the window's size will be changed to the closest one
	tracking.roi = aRoi;
	port.ops.roiResize(tracking.roi);
	tracking.roi.fitShiftRoi(aImage.asRoi());
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
}

void Tracker::update(const Tp::Image &aImage, bool aUpdatePsr)
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
	tracking.roi.fitShiftRoi(aImage.asRoi());  // Make sure the window fits the image. Adjust its coordinates while preserving the size
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
