//
// TestParallelOps.cpp
//
// Created on: Sep 20, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//


#include <Fft.h>
#include <embmosse/Port/MossePort.hpp>
#include <embmosse/Port/Task.hpp>
#include <embmosse/Port/Thread.hpp>
#include <embmosse/Port/Thread/StlThread.hpp>
#include <embmosse/Types/Tracking.hpp>
#include <embmosse/Types/Repr.hpp>
#include <embmosse/Util/Arithm/Arithm.hpp>
#include <embmosse/Util/Arithm/MemLayout.hpp>
#include <embmosse/Util/Ops/ThreadedOps.hpp>
#include <embmosse/Util/Ops/RawF32Ops.hpp>
#include <embmosse/Util/Ops/ParallelOps.hpp>
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
