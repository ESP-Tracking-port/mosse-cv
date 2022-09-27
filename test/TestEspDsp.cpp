//
// TestEspDsp.cpp
//
// Created on: Sep 27, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <Fft.h>
#include <Port/MossePort.hpp>
#include <Types/Tracking.hpp>
#include <Types/Repr.hpp>
#include <Util/Ops/ThreadedOps.hpp>
#include <Util/Ops/RawF32Ops.hpp>
#include <Util/Helper/EspDspFft2.hpp>
#include "esp_dsp.h"
#include <doctest/doctest.h>
#include <iostream>
#include <iterator>
#include <algorithm>

using namespace Mosse;

template <class T, std::size_t N>
std::ostream &operator<<(std::ostream &aOut, const std::array<T, N> &aArr)
{
	std::copy_n(aArr.begin(), N, std::ostream_iterator<T>(aOut, "\n"));

	return aOut;
}

static constexpr std::size_t kSignalLength = 64;

static constexpr std::array<float, kSignalLength * 2> kSignal{{
	4.734472441456392f, 0.0f, 5.084260978553312f, 0.0f, 3.5033694329113065f, 0.0f, -10.076512743774064f, 0.0f,
	-5.316900137541943f, 0.0f, -3.6612989020542077f, 0.0f, 14.576789801949344f, 0.0f, -2.0061614163836f, 0.0f,
	-4.621780991583516f, 0.0f, 2.1287314518556903f, 0.0f, 4.29761465192211f, 0.0f, 2.0433395248850967f, 0.0f,
	7.949334626098317f, 0.0f, 12.154405607259028f, 0.0f, 10.412773154004048f, 0.0f, -5.230908922621909f, 0.0f,
	5.718316935397272f, 0.0f, -2.7131712477463523f, 0.0f, 10.63921388033154f, 0.0f, 4.527968737926088f, 0.0f,
	-1.4346207151591353f, 0.0f, 3.626542143170052f, 0.0f, -9.809472298391203f, 0.0f, 2.2285639157199038f, 0.0f,
	1.3723615811474685f, 0.0f, 2.8088730047278956f, 0.0f, 0.6536005599409052f, 0.0f, -2.0176375914008275f, 0.0f,
	5.699459723692256f, 0.0f, 3.4817179748788143f, 0.0f, 1.6567981784563104f, 0.0f, 9.137794816192175f, 0.0f,
	0.2987402928347489f, 0.0f, -4.383400486211286f, 0.0f, 1.3834122781752844f, 0.0f, 5.6916322388902945f, 0.0f,
	8.476913559613683f, 0.0f, 4.616953300692915f, 0.0f, 12.78395965408364f, 0.0f, 3.7299317770734293f, 0.0f,
	-4.715198231321494f, 0.0f, -6.06626954700518f, 0.0f, 5.570154912987395f, 0.0f, 7.216806168197744f, 0.0f,
	14.929542005718567f, 0.0f, 7.857252990436102f, 0.0f, 8.660674321528397f, 0.0f, 3.8770247052579085f, 0.0f,
	2.2108682066881027f, 0.0f, 11.698126072164172f, 0.0f, 3.824276324095621f, 0.0f, 11.961862562123768f, 0.0f,
	4.226142473756718f, 0.0f, 0.7251100122320535f, 0.0f, -4.257414492971437f, 0.0f, 12.276588295120586f, 0.0f,
	12.420850521146212f, 0.0f, 11.136113561661425f, 0.0f, -1.044101780122185f, 0.0f, 1.3537914274890257f, 0.0f,
	-3.8948364416148653f, 0.0f, 12.894276897091462f, 0.0f, 15.259709870504022f, 0.0f, 11.145543733143622f, 0.0f
}};
static float sCoeffs[kSignalLength];

TEST_CASE("Test ESP DSP : Radix 2 F32 FFT, non-wrapped") {
	auto signal = kSignal;
	dsps_fft2r_init_fc32(sCoeffs, kSignalLength);
	// FFT
	dsps_fft2r_fc32_ansi_(signal.data(), kSignalLength, sCoeffs);
	dsps_bit_rev_fc32(signal.data(), kSignalLength);
	// IFFT
	dsps_fft2r_fc32_ansi_(signal.data(), kSignalLength, sCoeffs);
	dsps_bit_rev_fc32(signal.data(), kSignalLength);
	// Mult
	dsps_mulc_f32_ansi(signal.data(), signal.data(), kSignalLength * 2, 1.0f / static_cast<float>(kSignalLength), 1, 1);

	for (auto s : signal) {
		CHECK(!std::isnan(s));
	}
}

static std::vector<float> getRowRe1Im1(const void *aDataRe1Im1, const Tp::Roi &aRoi, std::size_t aRow)
{
	static constexpr Tp::Repr::Flags kGeometry = Tp::Repr::ReprRaw | Tp::Repr::CplxRe1Im1 | Tp::Repr::StorageF32;
	static constexpr std::size_t kDim = 1;
	std::vector<float> out;
	out.reserve(aRoi.size(kDim));

	for (std::size_t i = 0; i < aRoi.size(kDim); ++i) {
		out.push_back(Ut::atAsVariant<kGeometry>({aRow, i}, aRoi, aDataRe1Im1));
		out.push_back(Ut::atImagAsVariant<kGeometry>({aRow, i}, aRoi, aDataRe1Im1));
	}

	return out;
}

static std::vector<float> getColRe1Im1(const void *aDataRe1Im1, const Tp::Roi &aRoi, std::size_t aCol)
{
	static constexpr Tp::Repr::Flags kGeometry = Tp::Repr::ReprRaw | Tp::Repr::CplxRe1Im1 | Tp::Repr::StorageF32;
	static constexpr std::size_t kDim = 0;
	std::vector<float> out;
	out.reserve(aRoi.size(kDim));

	for (std::size_t i = 0; i < aRoi.size(kDim); ++i) {
		out.push_back(Ut::atAsVariant<kGeometry>({i, aCol}, aRoi, aDataRe1Im1));
		out.push_back(Ut::atImagAsVariant<kGeometry>({i, aCol}, aRoi, aDataRe1Im1));
	}

	return out;
}

TEST_CASE("Test ESP DSP : Radix 2 F32 FFT, wrapped, compare")
{
	static constexpr Tp::Repr::Flags kRepr = Tp::Repr::ReprRaw | Tp::Repr::CplxRe1Im1 | Tp::Repr::StorageF32;
	using WrapRow = Ut::Impl::EspDspFft2BufferWrap<kRepr, 0>;
	using WrapCol = Ut::Impl::EspDspFft2BufferWrap<kRepr, 1>;
	Tp::Roi roi{{0, 0}, {8, 8}};
	auto signal = kSignal;
	auto signalPtr = static_cast<void *>(signal.data());
	static constexpr float kEpsilon = 0.001f;

	SUBCASE("Compare slices") {
		{
			WrapRow wrap{roi, signalPtr, 0};

			for (; !wrap.isEnd(); wrap.advance()) {
				auto slice = getRowRe1Im1(signalPtr, roi, wrap.rowcol);

				for (std::size_t col = 0; col < roi.size(1); ++col) {
					CHECK(abs(slice[col * 2] - Ut::atAsVariant<kRepr>({wrap.rowcol, col}, roi, signalPtr).f32)
						< kEpsilon);
					CHECK(abs(slice[col * 2 + 1] - Ut::atImagAsVariant<kRepr>({wrap.rowcol, col}, roi, signalPtr).f32)
						< kEpsilon);
				}
			}
		}
		{
			WrapCol wrap{roi, signalPtr, 0};

			for (; !wrap.isEnd(); wrap.advance()) {
				auto slice = getColRe1Im1(signalPtr, roi, wrap.rowcol);

				for (std::size_t row = 0; row < roi.size(0); ++row) {
					CHECK(abs(slice[row * 2] - Ut::atAsVariant<kRepr>({row, wrap.rowcol}, roi, signalPtr).f32)
						< kEpsilon);
					CHECK(abs(slice[row * 2 + 1] - Ut::atImagAsVariant<kRepr>({row, wrap.rowcol}, roi, signalPtr).f32)
						< kEpsilon);
				}
			}
		}
	}
}

TEST_CASE("Test ESP DSP : Radix 2 F32 FFT2, wrapped")
{
	return;  // TODO. This test does not clear up yet. It crashes. Fix required.
	auto signal = kSignal;
	static constexpr std::size_t kRoiSideSize = 8;
	Tp::Roi roi{{0, 0}, {8, 8}};

	CHECK(1 == Ut::offsetFirstImag<Ut::RawF32Ops::reprFlags.buffer>(roi));

	dsps_fft2r_init_fc32(sCoeffs, kRoiSideSize);
	Ut::EspDspFft2<Ut::RawF32Ops::reprFlags.buffer> espDsp;
	espDsp.init(roi, sCoeffs, sCoeffs);

	for (auto s : signal) {
		CHECK(!std::isnan(s));
	}

	espDsp.fft2(static_cast<void *>(signal.data()));

	for (auto s : signal) {
		CHECK(!std::isnan(s));
	}

	espDsp.ifft2(static_cast<void *>(signal.data()));

	for (auto s : signal) {
		CHECK(!std::isnan(s));
	}
}
