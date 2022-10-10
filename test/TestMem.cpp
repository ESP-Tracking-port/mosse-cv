//
// TestMem.cpp
//
// Created on: Oct 10, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <embmosse/Mosse.hpp>
#include <embmosse/Util/Mem/DynRawMem.hpp>
#include <doctest/doctest.h>

TEST_CASE("Mem : Cropimage region, check if equal")
{
	static constexpr std::size_t kImageSideLen = 240;
	std::array<std::uint8_t, kImageSideLen * kImageSideLen> bufImage;
	Mosse::Ut::DynRawMem<Mosse::Ut::FpI16AbRawF32Ops::reprFlags.buffer, Mosse::Ut::FpI16AbRawF32Ops::reprFlags.matAb>
		mem;
	const Mosse::Tp::Roi roi{{120, 120}, {64, 64}};

	for (std::size_t i = 0; i < bufImage.size(); ++i) {
		bufImage[i] = static_cast<std::uint8_t>(i);
	}

	Mosse::Tp::Image image{bufImage.data(), kImageSideLen, kImageSideLen};
	mem.initImageWorkingArea(image, roi);
	Mosse::Tp::OffsetImage offsetImage{roi, static_cast<std::uint8_t *>(mem.imageWorkingArea())};
	bool eq = true;

	for (auto row = roi.origin(0); row < roi.origin(0) + roi.size(0); ++row) {
		for (auto col = roi.origin(1); col < roi.origin(1) + roi.size(1); ++col) {
			eq = eq && (image(row, col) == offsetImage(row, col));
		}
	}

	CHECK(eq);
}
