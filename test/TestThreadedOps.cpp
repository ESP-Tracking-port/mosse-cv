//
// TestThreadedOps.cpp
//
// Created on: Sep 19, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <Fft.h>
#include <portsrc/Port/MossePort.hpp>
#include <portsrc/Types/Tracking.hpp>
#include <portsrc/Types/Repr.hpp>
#include <portsrc/Util/Ops/ThreadedOps.hpp>
#include <portsrc/Util/Ops/RawF32Ops.hpp>
#include <doctest/doctest.h>
#include <MallocCounter.hpp>

// Only tests whether the `ThreadedOps` module is able to get successfully compiled.
TEST_CASE("ThreadedOps : Compilation") {
	std::array<std::uint8_t, 240*240> bufImg;
	std::array<float, 64 * 64 * 2> bufCplxCrop;
	Mosse::Tp::Roi roi{{10, 10}, {70, 70}};
	Mosse::Ut::RawF32Ops ops;
	Mosse::Ut::ThreadedOps threadedOps{ops};
	Mosse::Tp::Image image{bufImg.data(), 240, 240};
	ops.init(roi);
	{
		MallocCounter mc{};
		threadedOps.setExec(&Mosse::Ut::DecomposedOps::imageCropInto, image, static_cast<void *>(bufCplxCrop.data()));
		ohdebug(Test ThreadedOps : Compilation, MallocCounter::getPeakCount());
	}
}
