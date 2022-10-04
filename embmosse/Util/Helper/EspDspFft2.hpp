//
// EspDspFft2.hpp
//
// Created on: Sep 26, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_HELPER_ESPDSPFFT2_HPP_)
#define MOSSE_UTIL_HELPER_ESPDSPFFT2_HPP_

#include "embmosse/Types/Tracking.hpp"
#include "embmosse/Types/Repr.hpp"
#include "embmosse/Util/Arithm/MemLayout.hpp"
#include "embmosse/Util/Helper/ReTp.hpp"
#include "embmosse/Port/MossePort.hpp"
#include <type_traits>
#include <memory>
#if !MOSSE_PORTABLE
# include <dsps_fft2r.hpp>
# include <esp_dsp.h>
#endif  // !MOSSE_PORTABLE

ohdebuggroup("")

namespace Mosse {
namespace Ut {
namespace Impl {

inline bool coordIsIm(std::size_t coord)
{
	return coord & 1;
}

/// \brief ESP DSP library expects a complex buffer w/ ReImReIm layout type. EspDspFft2BuffeRWrapCols serves as
/// an adapter enabling column-wise iteration
/// \tparam D Dimension (0 or 1)
///
template <Tp::Repr::Flags F, std::size_t D>
struct EspDspFft2BufferWrap {
	static_assert(D == 0 || D == 1, "");
	Tp::Roi roi;
	void *buffer;
	Eigen::Index rowcol;

	inline void advance()
	{
		++rowcol;
	}

	inline bool isEnd() const
	{
		return rowcol >= roi.size(D);
	}

	template <std::size_t D1 = D>
	inline typename std::enable_if<D1 == 0, ReTp<F> &>::type operator[](std::size_t col)
	{
		if (coordIsIm(col)) {
			return *static_cast<ReTp<F> *>(atImag<F>({rowcol, col / 2}, roi, buffer));
		} else {
			return *static_cast<ReTp<F> *>(at<F>({rowcol, col / 2}, roi, buffer));
		}
	}

	template <std::size_t D1 = D>
	inline typename std::enable_if<D1 == 1, ReTp<F> &>::type operator[](std::size_t row)
	{
		if (coordIsIm(row)) {
			return *static_cast<ReTp<F> *>(atImag<F>({row / 2, rowcol}, roi, buffer));
		} else {
			return *static_cast<ReTp<F> *>(at<F>({row / 2, rowcol}, roi, buffer));
		}
	}
};

template <class T>
struct EspDspFftR2Callable;

template <>
struct EspDspFftR2Callable<float> {
#if !MOSSE_PORTABLE
	static constexpr auto init = dsps_fft2r_init_fc32;
	static constexpr auto mulc = dsps_mulc_f32;

	template <class T>
	struct Bitrev {
		static constexpr auto bitrev = dsps_bit_rev_fc32_ansi_step<T>;
	};

	template <class T>
	struct Fft {
		static constexpr auto fft = dsps_fft2r_fc32_ansi_step<T>;
	};
#endif // MOSSE_PORTABLE
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
#if !MOSSE_PORTABLE
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
#else
		(void)aRoi;
		(void)aRowsCoeffTable;
		(void)aColsCoeffTable;
#endif
	}

	void fft2(void *aBuffer) override
	{
#if !MOSSE_PORTABLE
		// Row-wise
		{
			using Wrap = Impl::EspDspFft2BufferWrap<F, 0>;
			Wrap wrap{roi, aBuffer, 0};

			for (; !wrap.isEnd(); wrap.advance()) {
				Impl::EspDspFftR2Callable<ReTp<F>>::template Fft<Wrap>::fft(wrap, roi.size(1), rowsCoeffTable);
				Impl::EspDspFftR2Callable<ReTp<F>>::template Bitrev<Wrap>::bitrev(wrap, roi.size(1));
			}
		}

		// Col-wise
		{
			using Wrap = Impl::EspDspFft2BufferWrap<F, 1>;
			Wrap wrap{roi, aBuffer, 0};

			for (; !wrap.isEnd(); wrap.advance()) {
				Impl::EspDspFftR2Callable<ReTp<F>>::template Fft<Wrap>::fft(wrap, roi.size(0), colsCoeffTable);
				Impl::EspDspFftR2Callable<ReTp<F>>::template Bitrev<Wrap>::bitrev(wrap, roi.size(0));
			}
		}
#else
		(void)aBuffer;
#endif
	}

	void ifft2(void *aBuffer) override
	{
#if !MOSSE_PORTABLE
		fft2(aBuffer);
		auto arg = static_cast<ReTp<F> *>(aBuffer);
		Impl::EspDspFftR2Callable<ReTp<F>>::mulc(arg, arg, roi.area() * 2, 1.0f / static_cast<float>(roi.area()), 1, 1); // TODO XXX
#else
		(void)aBuffer;
#endif
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
