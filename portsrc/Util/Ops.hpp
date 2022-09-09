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
private:
	struct Coeffs {
		float eta;
		float invEta;
	};
public:

	/// \brief Implementation MUST expect that it will be called multiple times during the tracking process and therefore
	/// optimize for unnecessary time and memory expenses. A descendant is, therefore, responsible for maintaining
	/// consistency.
	///
	/// \pre Tp::Roi is guaranteed to be of correct size. E.g. if a pre-defined set of window sizes is used, `aRoi` will
	/// be resized properly before passed into `init`.
	///
	void init(Tp::Roi aRoi);
	virtual void imageCropInto(Tp::Image aImageReal, void *aBufferComplex) = 0;  ///< Crop the image into buffer and perform preprocessing
	virtual void imagePreprocess(void *aCropComplex);  ///< Obsolete
	virtual void imageConvFftDomain(void *aioCropFft2Complex, void *aMatrixAcomlex, void *aMatrixBcomplex) = 0;
	virtual void fft2(void *aBufferComplex) = 0;
	virtual void ifft2(void *aBufferComplex) = 0;
	virtual ~Ops() = default;

	/// \brief Enables enforcing the use of a correct tracking window size. It is intended to only use a set of
	/// precompiled window sizes to spare computational expenses on Gaussian kernels and Hann windows.
	///
	/// \note The behavior described above is the only one that is supposed to be implmemented. However, this method can
	/// be virtualized and overriden, if the necessity comes.
	///
	void roiResize(Tp::Roi &);

	/// \brief Calculates max. value in a matrix.
	///
	/// \arg sum (optional) - stores sum of all the values. The result can be used later in `calcPsr`
	///
	virtual void maxReal(const void *aComplexBuffer, Tp::PointRowCol &aPeakPos, float *sum = nullptr) = 0;
	virtual float calcPsr(const void *aComplexBuffer, const Tp::PointRowCol &aPeak, float sumHint,
		Tp::PointRowCol aMask = {11, 11}) = 0;

	/// \brief Updates the A matrix (ref. to the MOSSE paper:
	/// https://www.cs.colostate.edu/~draper/papers/bolme_cvpr10.pdf)
	///
	/// \arg eta - learning rate. Authors of the paper suggest using `0.125`
	/// \arg aInitial - if false, a weighted element-wise sum of the current and the previous element will be used.
	///
	virtual void mataUpdate(void *aMatAcomplex, const void *aImageCropFftComplex, bool aInitial) = 0;
	virtual void matbUpdate(void *aMatBcomplex, const void *aImageCropFftComplex, bool aInitial) = 0;
protected:
	inline float eta() const
	{
		return coeffs.eta;
	}

	inline float invEta() const
	{
		return coeffs.invEta;
	}

	inline void setEta(float eta)
	{
		coeffs = {eta, 1.0f - eta};
	}

	const Tp::Roi &roi() const;
	virtual void initImpl();
	virtual const void *hannMatrix() = 0;  ///< Precompiled or generated (for test implementations) hann matrix
	virtual const void *gaussFft() = 0;  ///< Fouried-transformed precompiled or generated Gaussian matrix
private:
	Coeffs coeffs;
	Tp::Roi mRoi;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_OPS_HPP_
