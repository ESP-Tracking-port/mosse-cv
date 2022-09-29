//
// TestFpm.cpp
//
// Created on: Sep 28, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <Fft.h>
#include <Port/MossePort.hpp>
#include <Types/Tracking.hpp>
#include <Types/Repr.hpp>
#include <Util/Ops/ThreadedOps.hpp>
#include <Util/Ops/RawF32Ops.hpp>
#include <doctest/doctest.h>
#include <iostream>
#include <cnl/all.h>
#include <fpm/fixed.hpp>
#include <fpm/ios.hpp>

namespace TestFpm {

using Fixed16e4 = typename fpm::fixed<std::int16_t, std::int32_t, 4>;

void demo()
{
	Fixed16e4 fixed{15.32f};
	Fixed16e4 fixed2{3.12f};
	auto fixedOut = fixed * fixed2;

	ohdebug(TestFpm demo, fixedOut, fixedOut.raw_value());
}

}  // namespace TestFpm

TEST_CASE("Test FPM, demo")
{
	TestFpm::demo();
}
