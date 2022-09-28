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

TEST_CASE("Cnl") {
	auto x = cnl::scaled_integer<std::int16_t, cnl::power<-8>>{3.5123123};

	std::cout << "CNL" << x << std::endl;
}
