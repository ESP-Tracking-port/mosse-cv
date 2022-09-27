//
// test.cpp
//
// Created on: Sep 08, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <Types/Repr.hpp>
#include <Util/Arithm/MemLayout.hpp>
#include <Mosse.hpp>
#include <doctest/doctest.h>
#include <cstdint>
#include <array>
#include <OhDebug.hpp>
#include <cmath>

using namespace Mosse;

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
			CAPTURE(row);
			CAPTURE(col);
			CAPTURE(rowColToPlain(row, col, kCols, 1, 0));
			CHECK_EQ(map(row, col), arr[rowColToPlain(row, col, kCols, 1, 0)]);
		}
	}
}

static constexpr float kSig66 = -4.257414492971437f;
static constexpr float kSig77Im = 0.1f;
static constexpr float kSig23 = 4.527968737926088f;
static constexpr float kSig41Im = 1.3f;
static constexpr std::size_t kSignalLength = 64;
static constexpr std::array<float, kSignalLength * 2> kSignal{{
	4.734472441456392f, 0.0f, 5.084260978553312f, 0.0f, 3.5033694329113065f, 0.0f, -10.076512743774064f, 0.0f,
	-5.316900137541943f, 0.0f, -3.6612989020542077f, 0.0f, 14.576789801949344f, 0.0f, -2.0061614163836f, 0.0f,
	-4.621780991583516f, 0.0f, 2.1287314518556903f, 0.0f, 4.29761465192211f, 0.0f, 2.0433395248850967f, 0.0f,
	7.949334626098317f, 0.0f, 12.154405607259028f, 0.0f, 10.412773154004048f, 0.0f, -5.230908922621909f, 0.0f,
	5.718316935397272f, 0.0f, -2.7131712477463523f, 0.0f, 10.63921388033154f, 0.0f, kSig23, 0.0f,
	-1.4346207151591353f, 0.0f, 3.626542143170052f, 0.0f, -9.809472298391203f, 0.0f, 2.2285639157199038f, 0.0f,
	1.3723615811474685f, 0.0f, 2.8088730047278956f, 0.0f, 0.6536005599409052f, 0.0f, -2.0176375914008275f, 0.0f,
	5.699459723692256f, 0.0f, 3.4817179748788143f, 0.0f, 1.6567981784563104f, 0.0f, 9.137794816192175f, 0.0f,
	0.2987402928347489f, 0.0f, -4.383400486211286f, kSig41Im, 1.3834122781752844f, 0.0f, 5.6916322388902945f, 0.0f,
	8.476913559613683f, 0.0f, 4.616953300692915f, 0.0f, 12.78395965408364f, 0.0f, 3.7299317770734293f, 0.0f,
	-4.715198231321494f, 0.0f, -6.06626954700518f, 0.0f, 5.570154912987395f, 0.0f, 7.216806168197744f, 0.0f,
	14.929542005718567f, 0.0f, 7.857252990436102f, 0.0f, 8.660674321528397f, 0.0f, 3.8770247052579085f, 0.0f,
	2.2108682066881027f, 0.0f, 11.698126072164172f, 0.0f, 3.824276324095621f, 0.0f, 11.961862562123768f, 0.0f,
	4.226142473756718f, 0.0f, 0.7251100122320535f, 0.0f, kSig66, 0.0f, 12.276588295120586f, 0.0f,
	12.420850521146212f, 0.0f, 11.136113561661425f, 0.0f, -1.044101780122185f, 0.0f, 1.3537914274890257f, 0.0f,
	-3.8948364416148653f, 0.0f, 12.894276897091462f, 0.0f, 15.259709870504022f, 0.0f, 11.145543733143622f, kSig77Im
}};

TEST_CASE("Memory layout : ReIm getters")
{
	static constexpr Tp::Repr::Flags kReprFlags = Tp::Repr::CplxRe1Im1 | Tp::Repr::ReprRaw | Tp::Repr::StorageF32;
	const Tp::Roi roi{{0, 0}, {8, 8}};
	const void *data = static_cast<const void *>(kSignal.data());
	static constexpr float kEpsilon = 0.001f;
	CHECK(fabs(kSig77Im - Ut::atImagAsVariant<kReprFlags>({7, 7}, roi, data).f32) < kEpsilon);
	CHECK(fabs(kSig66 - Ut::atAsVariant<kReprFlags>({6, 6}, roi, data).f32) < kEpsilon);
	CHECK(fabs(kSig23 - Ut::atAsVariant<kReprFlags>({2, 3}, roi, data).f32) < kEpsilon);
	CHECK(fabs(kSig41Im - Ut::atImagAsVariant<kReprFlags>({4, 1}, roi, data).f32) < kEpsilon);
}
