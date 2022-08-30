//
// Ops.hpp
//
// Created on: Aug 29, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_OPS_HPP_)
#define MOSSE_UTIL_OPS_HPP_

namespace Mosse {
namespace Tp {

class Image;
class Roi;
class PointRowCol;

}  // namespace Tp

namespace Ut {

/// \brief A numeric representation may use fixed point, or some other compact representation. Therefore, all the
/// matrix operations are delegated to descendants of `Ops`
///
class Ops {
public:

	/// \brief Implementation MUST expect that it will be called multiple times during the tracking process and therefore
	/// optimize for unnecessary time and memory expenses.
	///
	virtual void init(Tp::Roi) = 0;
	virtual void imageCropInto(Tp::Image aImageReal, void *aBufferComplex) = 0;
	virtual void imagePreprocess(void *aCropComplex) = 0;
	virtual void imageConvFftDomain(void *aioCropFft2Complex, void *aMatrixAcomlex, void *aMatrixBcomplex);
	virtual void fft2(void *aBufferComplex) = 0;
	virtual void ifft2(void *aBufferComplex) = 0;

	/// \brief Calculates max. value in a matrix.
	///
	/// \arg sum (optional) - stores sum of all the values. The result can be used later in `calcPsr`
	///
	virtual void maxReal(const void *aComplexBuffer, Tp::PointRowCol &aPeakPos, float *sum = nullptr);
	float calcPsr(const void *aComplexBuffer, const Tp::PointRowCol &aPeak, float *sumHint = nullptr,
		Tp::PointRowCol aMask = {11, 11});

	/// \brief Updates the A matrix (ref. to the MOSSE paper:
	/// https://www.cs.colostate.edu/~draper/papers/bolme_cvpr10.pdf)
	///
	/// \arg eta - learning rate. Authors of the paper suggest using `0.125`
	/// \arg aInitial - if false, a weighted element-wise sum of the current and the previous element will be used.
	///
	virtual void mataUpdate(void *aMatAcomplex, const void *aImageCropFftComplex, float aEta, bool aInitial) = 0;
	virtual void matbUpdate(void *aMatBcomplex, const void *aImageCropFftComplex, float aEta, bool aInitial) = 0;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_OPS_HPP_
