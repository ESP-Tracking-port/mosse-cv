#ifndef _TRACKER_H_HH
#define _TRACKER_H_HH

#include "MossePort.hpp"
#include "Util/Ops.hpp"

# if !MOSSE_USE_OPENCV

namespace cv {

struct Rect {
};

struct Mat {
};

struct Size {
};

struct Rect2i {
};

}  // namespace cv

#endif

namespace Mosse {
namespace Ut {

class OpencvNativeRawF32Ops : public Ops {
public:
	using Ops::init;
	// Ops API
	OpencvNativeRawF32Ops();
	void imageCropInto(Tp::Image aImageReal, void *aBufferComplex) override;
	void imagePreprocess(void *aCropComplex) override;
	void imageConvFftDomain(void *aioCropFft2Complex, void *aMatrixAcomplex, void *aMatrixBcomplex) override;
	void fft2(void *aBufferComplex) override;
	void ifft2(void *aBufferComplex) override;
	void maxReal(const void *aComplexBuffer, Tp::PointRowCol &aPeakPos, float *sum = nullptr) override;
	float calcPsr(const void *aComplexBuffer, const Tp::PointRowCol &aPeak, float sumHint,
		Tp::PointRowCol aMask = {11, 11}) override;
	void mataUpdate(void *aMatAcomplex, const void *aImageCropFftComplex, bool aInitial) override;
	void matbUpdate(void *aMatBcomplex, const void *aImageCropFftComplex, bool aInitial) override;
	void initImpl() override;
	const void *hannMatrix() override;  ///< Precompiled hann matrix
	const void *gaussFft() override;  ///< Fouried-transformed precompiled Gaussian matrix
private:
	/// \tparam Open CV type, e.g. CV_8SC1
	///
	template <int Cvtype, class T>
	static cv::Mat bufferToMat(T aBuf, unsigned aRows, unsigned aCols)
	{
#if MOSSE_USE_OPENCV
		int sizes[2] = {static_cast<int>(aCols), static_cast<int>(aRows)};
		return cv::Mat{2, sizes, Cvtype, const_cast<void *>(static_cast<const void *>(aBuf))};
#else
		return {};
#endif
	}

	template <int Cvtype, class T>
	static cv::Mat bufferToMat(T aBuf, const Tp::Roi &aRoi)
	{
#if MOSSE_USE_OPENCV
		return bufferToMat<Cvtype>(aBuf, static_cast<int>(aRoi.size(0)), static_cast<int>(aRoi.size(1)));
#else
		return {};
#endif
	}

	static void matCvCopyInto(const cv::Mat &aMat, void *aOut);
	cv::Rect2i roiCv();
	void init(cv::Rect roi, const cv::Mat& image);
	cv::Rect update(const cv::Mat& image);
	void init_param();
	void train(const cv::Mat& image);
	cv::Mat imcrop(cv::Rect roi, const cv::Mat& image);
	cv::Mat fft(cv::Mat image, bool backwards = false);
	cv::Mat conj(const cv::Mat& image);
	cv::Mat preprocess(const cv::Mat& image);
	cv::Mat createHanningMats(int rows, int cols);
	cv::Mat rand_warp(const cv::Mat& image);
	cv::Mat convert(const cv::Mat& src);
	cv::Mat real(cv::Mat image);
	cv::Mat imag(cv::Mat image);
	cv::Mat complexDivision(cv::Mat a, cv::Mat b);
	cv::Mat complexMultiplication(cv::Mat a, cv::Mat b);
	double calculatePsr(const cv::Mat &aResponse);
private:
	float _sigma;
	float _eta;
	cv::Mat guassKernelMatrix;
	cv::Mat gauss_fft;
	cv::Mat Ai;
	cv::Mat Bi;
	cv::Mat Hi;
	cv::Mat fi;
	cv::Mat fi_fft;
	cv::Size init_sz;							/*初始化尺寸*/
	cv::Rect _roi;								/*每一帧跟踪结果*/
};
#endif

}  // namespace Ut
}  // namespace Mosse
