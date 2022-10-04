//
// TestApply.cpp
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
#include <iostream>

int max(int a, int b)
{
	return a > b ? a : b;
}

TEST_CASE("Apply") {
	std::tuple<int, int> args{42, 43};
	std::cout << Mosse::Ut::apply(max, args) << std::endl;
}
