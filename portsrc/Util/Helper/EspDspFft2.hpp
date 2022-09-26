//
// EspDspFft2.hpp
//
// Created on: Sep 26, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_HELPER_ESPDSPFFT2_HPP_)
#define MOSSE_UTIL_HELPER_ESPDSPFFT2_HPP_

#include "Types/Tracking.hpp"
#include "Types/Repr.hpp"
#include "Util/Arithm/MemLayout.hpp"
#include "Util/Helper/ReTp.hpp"
#include <dsps_fft2r.hpp>
#include <esp_dsp.h>
#include <type_traits>
#include <memory>

namespace Mosse {
namespace Ut {
namespace Impl {

inline bool coordIsIm(std::size_t coord)
{
	return coord & 1;
}

/// \brief ESP DSP library expects a complex buffer w/ ReImReIm layout type. EspDspFft2BuffeRWrapCols serves as
/// an adapter enabling column-wise iteration
///
template <Tp::Repr::Flags F>
struct EspDspFft2BufferWrapRow {
	Tp::Roi roi;
	void *buffer;
	std::size_t row;

	inline void nextRow()
	{
		++row;
	}

	inline bool isEnd() const
	{
		return row == roi.size(0);
	}

	inline ReTp<F> &operator[](std::size_t col)
	{
		if (coordIsIm(col)) {
			return *static_cast<ReTp<F> *>(atImag<F>({row, col}, roi, buffer));
		} else {
			return *static_cast<ReTp<F> *>(at<F>({row, col}, roi, buffer));
		}
	}
};

/// \brief ESP DSP library expects a complex buffer w/ ReImReIm layout type. EspDspFft2BuffeRWrapCols serves as
/// an adapter enabling row-wise iteration
///
template <Tp::Repr::Flags F>
struct EspDspFft2BufferWrapCol {
	Tp::Roi roi;
	void *buffer;
	std::size_t col;

	inline void nextCol()
	{
		++col;
	}

	inline bool isEnd()
	{
		return col == roi.size(1);
	}

	inline ReTp<F> &operator[](std::size_t row)
	{
		if (coordIsIm(col)) {
			return *static_cast<ReTp<F> *>(atImag<F>({row, col}, roi, buffer));
		} else {
			return *static_cast<ReTp<F> *>(at<F>({row, col}, roi, buffer));
		}
	}
};

template <class T>
struct EspDspFftR2Callable;

template <>
struct EspDspFftR2Callable<float> {
	static constexpr auto init = dsps_fft2r_init_fc32;
	static constexpr auto mulc = dsps_mulc_f32;

	template <class T>
	static constexpr auto bitrev = dsps_bit_rev_fc32_ansi_step<T>;

	template <class T>
	static constexpr auto fft = dsps_fft2r_fc32_ansi_step<T>;
};

}  // namespace Impl

struct EspDspFft2Base {
	virtual void fft2(void *) = 0;
	virtual void ifft2(void *) = 0;
};

template <Tp::Repr::Flags F>
class EspDspFft2 : public EspDspFft2Base {
public:
	EspDspFft2() = default;

	void init(const Tp::Roi &aRoi, const ReTp<F> *aRowsCoeffTable = nullptr, const ReTp<F> *aColsCoeffTable = nullptr)
	{
		roi = aRoi;
		rowsCoeffTable = aRowsCoeffTable;
		colsCoeffTable = aColsCoeffTable;

		if (nullptr == rowsCoeffTable) {
			rowsCoeffTableBuf = std::unique_ptr<ReTp<F>[]>{new ReTp<F>[roi.size(0)]};
			Impl::EspDspFftR2Callable<ReTp<F>>::init(rowsCoeffTableBuf.get(), roi.size(0));
			rowsCoeffTable = rowsCoeffTableBuf.get();
		}

		if (nullptr == colsCoeffTable) {
			colsCoeffTableBuf = std::unique_ptr<ReTp<F>[]>{new ReTp<F>[roi.size(1)]};
			Impl::EspDspFftR2Callable<ReTp<F>>::init(colsCoeffTableBuf.get(), roi.size(1));
			colsCoeffTable = colsCoeffTableBuf.get();
		}
	}

	void fft2(void *aBuffer) override
	{
		// Row-wise
		{
			using Wrap = typename Impl::EspDspFft2BufferWrapRow<F>;
			Wrap wrap{roi, aBuffer, 0};

			for (; !wrap.isEnd(); wrap.nextRow()) {
				Impl::EspDspFftR2Callable<ReTp<F>>::template fft<Wrap>(wrap, roi.size(0), rowsCoeffTable);
				Impl::EspDspFftR2Callable<ReTp<F>>::template bitrev<Wrap>(wrap, roi.size(0));
			}
		}

		// Col-wise
		{
			using Wrap = Impl::EspDspFft2BufferWrapCol<F>;
			Wrap wrap{roi, aBuffer, 0};

			for (; !wrap.isEnd(); wrap.nextCol()) {
				Impl::EspDspFftR2Callable<ReTp<F>>::template fft<Wrap>(wrap, roi.size(1), colsCoeffTable);
				Impl::EspDspFftR2Callable<ReTp<F>>::template bitrev<Wrap>(wrap, roi.size(1));
			}
		}
	}

	void ifft2(void *aBuffer)
	{
		fft2(aBuffer);
		auto arg = static_cast<ReTp<F> *>(aBuffer);
		Impl::EspDspFftR2Callable<ReTp<F>>::mulc(arg, arg, roi.area() * 2, 1.0f / static_cast<float>(roi.area()), 1, 1); // TODO XXX
	}
private:
	Tp::Roi roi = {};
	const ReTp<F> *rowsCoeffTable = nullptr;
	const ReTp<F> *colsCoeffTable = nullptr ;
	std::unique_ptr<ReTp<F>[]> rowsCoeffTableBuf = {};
	std::unique_ptr<ReTp<F>[]> colsCoeffTableBuf = {};
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_HELPER_ESPDSPFFT2_HPP_
