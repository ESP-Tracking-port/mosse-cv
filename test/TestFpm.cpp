//
// TestFpm.cpp
//
// Created on: Sep 28, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <Fft.h>
#include <embmosse/Port/MossePort.hpp>
#include <embmosse/Types/Tracking.hpp>
#include <embmosse/Types/Repr.hpp>
#include <embmosse/Util/Ops/ThreadedOps.hpp>
#include <embmosse/Util/Ops/RawF32Ops.hpp>
#include <doctest/doctest.h>
#include <iostream>
#include <fpm/fixed.hpp>
#include <fpm/ios.hpp>

namespace TestFpm {

using Fixed16e4 = typename fpm::fixed<std::int16_t, std::int32_t, 8>;

void demo()
{
	Fixed16e4 fixed{15.32f};
	Fixed16e4 fixed2{3.12f};
	Fixed16e4 epsilon{0.003f};
	auto fixedOut = fixed * fixed2;

	ohdebug(TestFpm demo, fixedOut, fixedOut.raw_value(), epsilon);
}

}  // namespace TestFpm

TEST_CASE("Test FPM, demo")
{
	TestFpm::demo();
}
