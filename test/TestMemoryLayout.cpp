//
// test.cpp
//
// Created on: Sep 08, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <Types/Repr.hpp>
#include <Mosse.hpp>
#include <doctest/doctest.h>
#include <cstdint>
#include <array>

template <class T, std::size_t Rows, std::size_t Cols, std::size_t Depth>
void arrayInit(std::array<T, Rows * Cols * Depth> &aArr)
{
	auto ***arr2d = reinterpret_cast<T ***>(aArr.data());
	T counter = static_cast<T>(0);

	for (std::size_t row = 0; row < Rows; ++row) {
		for (std::size_t col = 0; col < Cols; ++col) {
			for (std::size_t dep = 0; dep < Depth; ++dep) {
				arr2d[row][col][dep] = counter + static_cast<T>(10) * static_cast<T>(dep);
				counter += static_cast<T>(1);
			}
		}
	}
}

TEST_CASE_TEMPLATE("Memory layout : lower-level memory accessing functions", T, float, std::int16_t)
{
	constexpr std::size_t kRows = 3;
	constexpr std::size_t kCols = 10;
	std::array<T, kRows * kCols * 2> arr{{static_cast<T>(0)}};
	T *begin = arr.data();
	const T *cbegin = arr.data();
	const Mosse::Tp::Roi roi{{0, 0}, {kRows, kCols}};

	SUBCASE("at") {
		SUBCASE("real array") {
			constexpr auto kRepr = Mosse::Tp::Repr::template Storage<T>::value | Mosse::Tp::Repr::ReprRaw;
			CHECK_EQ(static_cast<void *>(Mosse::Ut::at<kRepr>(3, begin)), begin + 3);
			CHECK_EQ(static_cast<const void *>(Mosse::Ut::at<kRepr>(2, cbegin)), cbegin + 2);
		}
	}

	SUBCASE("offset") {
		SUBCASE("first imag") {
			SUBCASE("CplxRe1Im1") {
				constexpr auto kRepr = Mosse::Tp::Repr::Storage<T>::value | Mosse::Tp::Repr::ReprRaw
					| Mosse::Tp::Repr::CplxRe1Im1;
				CHECK_EQ(Mosse::Ut::offsetFirstImag<kRepr>(roi), 1);
			}
			SUBCASE("CplxRenImn") {
				constexpr auto kRepr = Mosse::Tp::Repr::Storage<T>::value | Mosse::Tp::Repr::ReprRaw
					| Mosse::Tp::Repr::CplxRenImn;
				CHECK_EQ(Mosse::Ut::offsetFirstImag<kRepr>(roi), roi.cols() * roi.rows());
			}
		}
	}

	SUBCASE("strideInner") {
		SUBCASE("CplxRe1Im1") {
			constexpr auto kRepr = Mosse::Tp::Repr::Storage<T>::value | Mosse::Tp::Repr::ReprRaw
				| Mosse::Tp::Repr::CplxRe1Im1;
			CHECK_EQ(Mosse::Ut::strideInner<kRepr>(), 2);
		}
		SUBCASE("Non-complex array") {
			constexpr auto kRepr = Mosse::Tp::Repr::Storage<T>::value | Mosse::Tp::Repr::ReprRaw;
			CHECK_EQ(Mosse::Ut::strideInner<kRepr>(), 1);
		}
		SUBCASE("CplxRenImn") {
			constexpr auto kRepr = Mosse::Tp::Repr::Storage<T>::value | Mosse::Tp::Repr::ReprRaw
				| Mosse::Tp::Repr::CplxRenImn;
			CHECK_EQ(Mosse::Ut::strideInner<kRepr>(), 1);
		}
	}
}

TEST_CASE("Memory layout : eigen wrappers")
{
	constexpr auto kRepresentationFlags = Mosse::Tp::Repr::ReprRaw | Mosse::Tp::Repr::CplxRe1Im1
		| Mosse::Tp::Repr::StorageF32;
	constexpr float kCounterMult = 10.0f;
	constexpr auto kRows = 5;
	constexpr auto kCols = 5;
	float matrix[kRows][kCols * 2] = {0.f};
	float counter = 0.0f;

	for (unsigned row = 0; row < kRows; ++row) {
		for (unsigned col = 0; col < kCols; col += 2) {
			matrix[row][col] = counter;
			matrix[row][col + 1] = counter * kCounterMult;
			counter += 1.0f;
		}
	}

	auto map = Mosse::Ut::makeEigenMap<kRepresentationFlags>(static_cast<void *>(matrix), {{0, 0}, {kRows, kCols}});
	auto mapImag = Mosse::Ut::makeEigenMapImag<kRepresentationFlags>(static_cast<void *>(matrix), {{0, 0},
		{kRows, kCols}});

	for (unsigned row = 0; row < kRows; ++row) {
		for (unsigned col = 0; col < kCols; ++col) {
			CAPTURE(map(row, col));
			CAPTURE(row);
			CAPTURE(col);
			CHECK_EQ(map(row, col), matrix[row][col]);
			CHECK_EQ(mapImag(row, col), matrix[row][col + 1]);
		}
	}
}
