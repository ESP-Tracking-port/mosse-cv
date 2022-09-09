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
#include <OhDebug.hpp>

#define foreachrowcol(rowvar, colvar, rows, cols) \
	for (std::size_t rowvar = 0; rowvar < rows; ++rowvar) \
		for (std::size_t colvar = 0; colvar < cols; ++colvar)

template <class T, std::size_t Rows, std::size_t Cols, std::size_t Depth, std::size_t N>
void arrayInit(std::array<T, N> &aArr)
{
	T counter = static_cast<T>(0);

	for (std::size_t row = 0; row < Rows; ++row) {
		for (std::size_t col = 0; col < Cols; ++col) {
			for (std::size_t dep = 0; dep < Depth; ++dep) {
				aArr[row * Cols * Depth + col * Depth + dep] = counter + static_cast<T>(10) * static_cast<T>(dep);
				counter += static_cast<T>(1);
			}
		}
	}
}

inline std::size_t rowColToPlain(std::size_t aRow, std::size_t aCol, std::size_t aCols, std::size_t aStride,
	std::size_t aOffset)
{
	return aCol * aStride + aRow * aCols * aStride;
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
			constexpr auto kRepr = Mosse::Tp::Repr::template Storage<T>::value | Mosse::Tp::Repr::ReprLog2;
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

TEST_CASE_TEMPLATE("Memory layout : eigen wrappers", T, float, std::int16_t)
{
	constexpr std::size_t kRows = 3;
	constexpr std::size_t kCols = 10;
	std::array<T, kRows * kCols * 2> arr{{static_cast<T>(0)}};
	const Mosse::Tp::Roi roi{{0, 0}, {kRows, kCols}};

	SUBCASE("Non-complex array") {
		constexpr auto kRepresentationFlags = Mosse::Tp::Repr::ReprRaw | Mosse::Tp::Repr::Storage<T>::value;
		auto map = Mosse::Ut::makeEigenMap<kRepresentationFlags>(static_cast<void *>(arr.data()), {{0, 0},
			{kRows, kCols}});
		arrayInit<T, kRows, kCols, 1>(arr);

		foreachrowcol(row, col, kRows, kCols) {
			ohdebug(, arr[rowColToPlain(row, col, kCols, 1, 0)], row, col);
			ohdebug(, map(row, col), row, col);
		}

		foreachrowcol(row, col, kRows, kCols) {
			CAPTURE(row);
			CAPTURE(col);
			CAPTURE(rowColToPlain(row, col, kCols, 1, 0));
			CHECK_EQ(map(row, col), arr[rowColToPlain(row, col, kCols, 1, 0)]);
		}
	}
}
