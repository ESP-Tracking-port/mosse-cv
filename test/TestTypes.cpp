//
// TestTypes.cpp
//
// Created on: Sep 22, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <embmosse/Types/Repr.hpp>
#include <embmosse/Mosse.hpp>
#include <doctest/doctest.h>
#include <cstdint>
#include <array>
#include <OhDebug.hpp>
#include <MallocCounter.hpp>

TEST_CASE("Test Types : Image copying, memory profiling")
{
	std::array<std::uint8_t, 64 * 64> buf {{0}};
	Mosse::Tp::Image image{buf.data(), 64, 64};
	MallocCounter counter{};
	Mosse::Tp::Image *imageCopy = new Mosse::Tp::Image{image};
	ohdebug("Test Types : Image copying memory profiling", MallocCounter::getPeakCount());
}
