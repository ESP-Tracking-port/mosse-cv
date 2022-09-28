//
// TestApply.cpp
//
// Created on: Sep 19, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

// Water-probe, getting familiar w/ the CNL library

#include <Fft.h>
#include <Port/MossePort.hpp>
#include <Types/Tracking.hpp>
#include <Types/Repr.hpp>
#include <Util/Ops/ThreadedOps.hpp>
#include <Util/Ops/RawF32Ops.hpp>
#include <doctest/doctest.h>
#include <iostream>
#include <cnl/all.h>

void demo()
{
	static constexpr float kNum = 3.53f;
	auto x = cnl::scaled_integer<int, cnl::power<-3>>{kNum};
	ohdebug(cnl demo, kNum, to_rep(x), x, x / 2, x / 3, x * x);
	auto xraw = to_rep(x);
	auto x2 = cnl::scaled_integer<int, cnl::power<-3>>{xraw};
	ohdebug(cnl demo, x2);
}

TEST_CASE("Cnl") {
	demo();
}
