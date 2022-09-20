//
// TestParallelOps.cpp
//
// Created on: Sep 20, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//


#include <Fft.h>
#include <Port/MossePort.hpp>
#include <Port/Task.hpp>
#include <Port/Thread.hpp>
#include <Port/Thread/StlThread.hpp>
#include <Types/Tracking.hpp>
#include <Types/Repr.hpp>
#include <Util/Arithm/Arithm.hpp>
#include <Util/Arithm/MemLayout.hpp>
#include <Util/Ops/ThreadedOps.hpp>
#include <Util/Ops/RawF32Ops.hpp>
#include <Util/Ops/ParallelOps.hpp>
#include <doctest/doctest.h>
#include <functional>

// Only tests whether the `ThreadedOps` module is able to get successfully compiled.
TEST_CASE("ThreadedOps : Compilation") {
	Mosse::Port::StlThread thread;
	Mosse::Ut::RawF32Ops rawOps1;
	Mosse::Ut::RawF32Ops rawOps2;
	Mosse::Ut::Arithm<Mosse::Ut::RawF32Ops::reprFlags.buffer> bufferArithm;
	Mosse::Ut::MemLayout<Mosse::Ut::RawF32Ops::reprFlags.buffer> bufferMemLayout;
	Mosse::Ut::ParallelOps ops{{std::ref<Mosse::Ut::DecomposedOps>(rawOps1),
		std::ref<Mosse::Ut::DecomposedOps>(rawOps2)}, thread, bufferArithm, bufferMemLayout};
}
