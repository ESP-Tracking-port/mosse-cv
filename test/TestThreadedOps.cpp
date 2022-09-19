//
// TestThreadedOps.cpp
//
// Created on: Sep 19, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <Fft.h>
#include <Port/MossePort.hpp>
#include <Types/Tracking.hpp>
#include <Types/Repr.hpp>
#include <Util/Ops/ThreadedOps.hpp>
#include <Util/Ops/RawF32Ops.hpp>
#include <doctest/doctest.h>

// Only tests whether the `ThreadedOps` module is able to get successfully compiled.
TEST_CASE("ThreadedOps : Compilation") {
	Mosse::Ut::RawF32Ops ops;
	Mosse::Ut::ThreadedOps threadedOps{ops};
	threadedOps.setExec(&Mosse::Ut::Ops::mataUpdate, nullptr, nullptr, true);
}
