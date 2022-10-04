//
// TestParallelOps.cpp
//
// Created on: Sep 20, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//


#include <Fft.h>
#include <portsrc/Port/MossePort.hpp>
#include <portsrc/Port/Task.hpp>
#include <portsrc/Port/Thread.hpp>
#include <portsrc/Port/Thread/StlThread.hpp>
#include <portsrc/Types/Tracking.hpp>
#include <portsrc/Types/Repr.hpp>
#include <portsrc/Util/Arithm/Arithm.hpp>
#include <portsrc/Util/Arithm/MemLayout.hpp>
#include <portsrc/Util/Ops/ThreadedOps.hpp>
#include <portsrc/Util/Ops/RawF32Ops.hpp>
#include <portsrc/Util/Ops/ParallelOps.hpp>
#include <doctest/doctest.h>
#include <functional>

// Only tests whether the `ThreadedOps` module is able to get successfully compiled.
TEST_CASE("ParallelOps : Compilation") {
	Mosse::Port::StlThread thread;
	Mosse::Ut::RawF32Ops rawOps1;
	Mosse::Ut::RawF32Ops rawOps2;
	Mosse::Ut::Arithm<Mosse::Ut::RawF32Ops::reprFlags.buffer> bufferArithm;
	Mosse::Ut::MemLayout<Mosse::Ut::RawF32Ops::reprFlags.buffer> bufferMemLayout;
}
