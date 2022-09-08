//
// test.cpp
//
// Created on: Sep 08, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <doctest/doctest.h>
#include "Mosse.hpp"

TEST_CASE("Ut real/imaginary mapping") {
	constexpr auto kRepresentationFlags = Mosse::Tp::Repr::ReprRaw | Mosse::Tp::Repr::CplxRe1Im1
		| Mosse::Tp::Repr::StorageF32;
	constexpr float kCounterMult = 10.0f;
	constexpr auto kRows = 5;
	constexpr auto kCols = 3;
	float matrix[kRows][kCols * 2] = {0.f};

	float counter = 0.0f;

	for (unsigned row = 0; row < kRows; ++row) {
		for (unsigned col = 0; col < kCols; col += 2) {
			matrix[row][col] = counter;
			matrix[row][col + 1] = counter * kCounterMult;
		}
	}

	auto map = Mosse::Ut::makeEigenMap<kRepresentationFlags>(static_cast<void *>(matrix), {{0, 0}, {kRows, kCols}});
	auto mapImag = Mosse::Ut::makeEigenMapImag<kRepresentationFlags>(static_cast<void *>(matrix), {{0, 0},
		{kRows, kCols}});

	for (unsigned row = 0; row < kRows; ++row) {
		for (unsigned col = 0; col < kCols; ++col) {
			ohdebugassert(testUtRealImaginaryMapping, map(row, col) == matrix[row][col]
				&& mapImag(row, col) == matrix[row][col + 1], map(row, col), mapImag(row, col), matrix[row][col],
				matrix[row][col + 1]);
		}
	}
}