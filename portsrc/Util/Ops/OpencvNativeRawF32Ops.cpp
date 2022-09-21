// TODO CV stores multichannel matrices plane-by-plane

#include "Port/MossePort.hpp"
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <MosseApi.hpp>
#include "OpencvNativeRawF32Ops.hpp"
#include "Util/Helper/PrecompiledMatrixHelper.hpp"

namespace Mosse {
namespace Ut {

static Ut::PrecompiledMatrixHelper<float> sPrecompiledMatrixHelper{Mosse::getGaussKernelFft3d,
	Mosse::getHann};

OpencvNativeRawF32Ops::OpencvNativeRawF32Ops() : Ops()
{
}

void OpencvNativeRawF32Ops::imageCropInto(Tp::Image aImageReal, void *aBufferComplex)
{
#if MOSSE_USE_OPENCV
# if 1
	auto image = bufferToMat<CV_8UC1>(aImageReal.data(), aImageReal.rows(), aImageReal.cols());
	image = imcrop(_roi, image);
	image = preprocess(image);

	cv::Mat planes[] = {cv::Mat_<float>(image), cv::Mat_<float>::zeros(image.size())};
	cv::merge(planes, 2, image);
	assert(image.type() == CV_32FC2);
	matCvCopyInto(image, aBufferComplex);
# endif  // # if 1
#else
	(void)aImageReal;
	(void)aBufferComplex;
#endif
}

void OpencvNativeRawF32Ops::imagePreprocess(void *aCropComplex)
{
	(void)aCropComplex;
}

void OpencvNativeRawF32Ops::imageConvFftDomain(void *aioCropFft2Complex, void *aMatrixAcomplex, void *aMatrixBcomplex)
{
#if MOSSE_USE_OPENCV
	auto out = bufferToMat<CV_32FC2>(aioCropFft2Complex, roi());
	auto mata = bufferToMat<CV_32FC2>(aMatrixAcomplex, roi());
	auto matb = bufferToMat<CV_32FC2>(aMatrixBcomplex, roi());

	out = complexMultiplication(complexDivision(mata, matb), out);
	matCvCopyInto(out, aioCropFft2Complex);
#else
	(void)aioCropFft2Complex;
	(void)aMatrixAcomplex;
	(void)aMatrixBcomplex;
#endif
}

void OpencvNativeRawF32Ops::fft2(void *aBufferComplex)
{
#if MOSSE_USE_OPENCV
	auto mat = bufferToMat<CV_32FC2>(aBufferComplex, roi());
	mat = fft(mat);
	matCvCopyInto(mat, aBufferComplex);
#else
	(void)aBufferComplex;
#endif
}

void OpencvNativeRawF32Ops::ifft2(void *aBufferComplex)
{
#if MOSSE_USE_OPENCV
	auto mat = bufferToMat<CV_32FC2>(aBufferComplex, roi());
	mat = fft(mat, true);
	matCvCopyInto(mat, aBufferComplex);
#else
	(void)aBufferComplex;
#endif
}

void OpencvNativeRawF32Ops::maxReal(const void *aBufferComplex, Tp::PointRowCol &aPeakPos, float *aSum)
{
#if MOSSE_USE_OPENCV
	cv::Mat response = bufferToMat<CV_32FC2>(aBufferComplex, roi());
	response = real(response);
	response *= 255.0;

	cv::Mat resp_cv8u = cv::Mat_<unsigned char>(response);
	cv::Point ps;
	cv::minMaxLoc(resp_cv8u, NULL, NULL, NULL, &ps);
	aPeakPos = {ps.y, ps.x};

#else
	(void)aBufferComplex;
	(void)aPeakPos;
	(void)aSum;
#endif
}

float OpencvNativeRawF32Ops::calcPsr(const void *aBufferComplex, const Tp::PointRowCol &aPeak, float aSumHint,
	Tp::PointRowCol aMask)
{
#if MOSSE_USE_OPENCV
	return 0.0f;
#else
	(void)aBufferComplex;
	(void)aPeak;
	(void)aSumHint;
	return 0.0f;
#endif
}

void OpencvNativeRawF32Ops::mataUpdate(void *aMatAcomplex, const void *aImageCropFftComplex, bool aInitial)
{
#if MOSSE_USE_OPENCV
	auto gaussfft = bufferToMat<CV_32FC2>(gaussFft(), roi());
	auto imagefft = bufferToMat<CV_32FC2>(aImageCropFftComplex, roi());
	auto mata = bufferToMat<CV_32FC2>(aMatAcomplex, roi());
//	ohdebug(OpencvNativeRawF32Ops::mataUpdate, mata, eta());

	if (aInitial) {
		mata = eta() * complexMultiplication(gaussfft, conj(imagefft));
	} else {
		mata = eta() * complexMultiplication(gaussfft, conj(imagefft)) + invEta() * mata;
	}

	matCvCopyInto(mata, aMatAcomplex);
#else
	(void)aMatAcomplex;
	(void)aImageCropFftComplex;
	(void)aInitial;
#endif
}

void OpencvNativeRawF32Ops::matbUpdate(void *aMatBcomplex, const void *aImageCropFftComplex, bool aInitial)
{
#if MOSSE_USE_OPENCV
	auto imagefft = bufferToMat<CV_32FC2>(aImageCropFftComplex, roi());
	auto matb = bufferToMat<CV_32FC2>(aMatBcomplex, roi());

	if (aInitial) {
		matb = eta() * complexMultiplication(imagefft, conj(imagefft));
	} else {
		matb = eta() * complexMultiplication(imagefft, conj(imagefft)) + invEta() * matb;
	}

	matCvCopyInto(matb, aMatBcomplex);
#else
	(void)aMatBcomplex;
	(void)aImageCropFftComplex;
	(void)aInitial;
#endif
}

void OpencvNativeRawF32Ops::initImpl()
{
#if MOSSE_USE_OPENCV
	sPrecompiledMatrixHelper.update(roi());
	setEta(0.125f);
	_roi = roiCv();
#else
#endif
}

const void *OpencvNativeRawF32Ops::hannMatrix()
{
#if MOSSE_USE_OPENCV
	return sPrecompiledMatrixHelper.hann();
#else
	return nullptr;
#endif
}

const void *OpencvNativeRawF32Ops::gaussFft()
{
#if MOSSE_USE_OPENCV
	return sPrecompiledMatrixHelper.gauss();
#else
	return nullptr;
#endif
}

void OpencvNativeRawF32Ops::matCvCopyInto(const cv::Mat &aMat, void *aOut)
{
#if MOSSE_USE_OPENCV
	const std::size_t size = aMat.elemSize() * aMat.rows * aMat.cols;
	std::copy_n(static_cast<std::uint8_t *>(aMat.data), size, static_cast<std::uint8_t *>(aOut));
#else
	(void)aMat;
	(void)aOut;
#endif
}

cv::Rect2i OpencvNativeRawF32Ops::roiCv()
{
#if MOSSE_USE_OPENCV
	return {static_cast<int>(roi().origin(1)), static_cast<int>(roi().origin(0)), static_cast<int>(roi().size(1)),
		static_cast<int>(roi().size(0))};
#else
	return {};
#endif
}

void OpencvNativeRawF32Ops::init_param()
{
	_sigma = 100;
	_eta = 0.125;
}

cv::Mat OpencvNativeRawF32Ops::imcrop(cv::Rect roi, const cv::Mat& image)
{
#if MOSSE_USE_OPENCV
	return image(roi);
#else
	(void)roi;
	(void)image;
#endif
}

cv::Mat OpencvNativeRawF32Ops::fft(cv::Mat image, bool backwards)
{
#if MOSSE_USE_OPENCV
	if(image.channels() == 1)
	{
		cv::Mat planes[] = {cv::Mat_<float>(image), cv::Mat_<float>::zeros(image.size())};
		cv::merge(planes, 2, image);
	}
	cv::dft(image, image, backwards ? (cv::DFT_INVERSE | cv::DFT_SCALE) : 0);

	return image;
#else
	(void)image;
	(void)backwards;

	return {};
#endif
}

cv::Mat OpencvNativeRawF32Ops::conj(const cv::Mat& image)
{
#if MOSSE_USE_OPENCV
	assert(image.channels() == 2);
	cv::Mat mat[2];
	cv::split(image, mat);
	mat[1] *= -1;
	cv::Mat res;
	cv::merge(mat, 2, res);
	return res;
#else
	(void)image;

	return {};
#endif
}

cv::Mat OpencvNativeRawF32Ops::createHanningMats(int rows, int cols)
{
#if MOSSE_USE_OPENCV
	cv::Mat hann1t = cv::Mat(cv::Size(cols, 1), CV_32F, cv::Scalar(0));
	cv::Mat hann2t = cv::Mat(cv::Size(1, rows), CV_32F, cv::Scalar(0));

	for(int i = 0; i < hann1t.cols; i++)
		hann1t.at<float>(0, i) = 0.5 * (1 - std::cos(2 * CV_PI * i / (hann1t.cols - 1)));
	for(int i = 0; i < hann2t.rows; i++)
		hann2t.at<float>(i, 0) = 0.5 * (1 - std::cos(2 * CV_PI * i / (hann2t.rows - 1)));

	cv::Mat hann2d = hann2t * hann1t;
	return hann2d;
#else
	(void)rows;
	(void)cols;
#endif
}

cv::Mat OpencvNativeRawF32Ops::preprocess(const cv::Mat& image)
{
#if MOSSE_USE_OPENCV
	cv::Mat win = bufferToMat<CV_32FC1>(hannMatrix(), roi());
//	cv::Mat win = createHanningMats(image.rows, image.cols);
	float eps = 1e-5;
	cv::Mat img = image + cv::Scalar::all(1);
	img = cv::Mat_<float>(img);
	cv::log(img, img);
	cv::Scalar mean, std;
	cv::meanStdDev(img, mean, std);
	img = (img - cv::Scalar::all(mean[0])) / (std[0] + eps);
	return img.mul(win);
#else
	(void)image;
	return {};
#endif
}

cv::Mat OpencvNativeRawF32Ops::rand_warp(const cv::Mat& image)
{
#if MOSSE_USE_OPENCV
	srand((unsigned)time(NULL));
	float a = -180.0 / 16;
	float b = -a;
	float rand_v_1 = rand() % 101 / 100.0;
	float r = a + (b - 1) * rand_v_1;

	float rand_v_2 = rand() % 101 / 100.0;
	float scale = 1 - 0.1 + 0.2 * rand_v_2;

	cv::Mat rotate_image = cv::Mat::zeros(image.size(), image.type());
	cv::Point center = cv::Point(image.cols/2, image.rows/2);
	cv::Mat rot_mat = cv::getRotationMatrix2D(center, double(r), double(scale));
	cv::warpAffine(image, rotate_image, rot_mat, image.size());

	return rotate_image;
#else
	(void)image;

	return {};
#endif
}

cv::Mat OpencvNativeRawF32Ops::complexMultiplication(cv::Mat a, cv::Mat b)
{
#if MOSSE_USE_OPENCV
	std::vector<cv::Mat> pa;
	std::vector<cv::Mat> pb;
	cv::split(a, pa);
	cv::split(b, pb);

	std::vector<cv::Mat> pres;
	pres.push_back(pa[0].mul(pb[0]) - pa[1].mul(pb[1]));
	pres.push_back(pa[0].mul(pb[1]) + pa[1].mul(pb[0]));

	cv::Mat res;
	cv::merge(pres, res);

	return res;
#else
	(void)a;
	(void)b;

	return {};
#endif
}

/// \brief PSR can be used as a tracking quality metric enabling use of threshold-based tracking failure detection.
///
/// PSR is calculated as $\over {max_response - mean} {stdev}$
///
/// When calculating mean and std. dev., a 11x11 window should be excluded from the response (according to the MOSSE
/// paper)
///
double OpencvNativeRawF32Ops::calculatePsr(const cv::Mat &aResponse)
{

#if MOSSE_USE_OPENCV
	// Get max response value
	double maxValue = 0.0f;
	cv::minMaxLoc(aResponse, nullptr, &maxValue, nullptr, nullptr);

	// Get max stats w/ an account for the 11x11 window
	cv::Scalar mean;
	cv::Scalar stddev;
	cv::Rect2i exclusionArea{aResponse.size().width / 2 - 5, aResponse.size().height / 2 - 5, 11, 11};
	cv::Mat mask = cv::Mat::ones(aResponse.size[0], aResponse.size[1], CV_8UC1);
	mask(exclusionArea) = 0;
	cv::meanStdDev(aResponse, mean, stddev, mask);

	return (maxValue - mean[0]) / stddev[0];
#else
	(void)aResponse;

	return {};
#endif
}

void OpencvNativeRawF32Ops::init(cv::Rect roi, const cv::Mat& gray)
{
#if MOSSE_USE_OPENCV
	init_param();
	unsigned rows = roi.height;
	unsigned cols = roi.width;
	cv::Point center = cv::Point(roi.x+roi.width/2, roi.y+roi.height/2);
	Mosse::getClosestWindow(rows, cols);
	_roi = cv::Rect2i(center.x - cols / 2, center.y - rows / 2, cols, rows);
	roi = _roi;
	cv::Mat gray_crop = imcrop(roi, gray);
	const int sizes[3] = {static_cast<int>(rows), static_cast<int>(cols), 2};
	void *gauss_fft_raw_3d = const_cast<void *>(static_cast<const void *>(Mosse::getGaussKernelFft3d(rows, cols)));
	assert(gauss_fft_raw_3d != nullptr);
	gauss_fft = cv::Mat(2, sizes, CV_32FC2, gauss_fft_raw_3d);
	init_sz.width = gauss_fft.cols;
	init_sz.height = gauss_fft.rows;
	fi = preprocess(gray_crop);
	fi_fft = fft(fi);
	Ai = complexMultiplication(gauss_fft, conj(fi_fft));
	Bi = complexMultiplication(fi_fft, conj(fi_fft));
	int N = 128;

	for (int i = 0; i < N; i++) {
		fi = preprocess(rand_warp(gray_crop));
		fi_fft = fft(fi);
		Ai += complexMultiplication(gauss_fft, conj(fi_fft));
		Bi += complexMultiplication(fi_fft, conj(fi_fft));
	}

	Ai *= _eta;
	Bi *= _eta;
#else
	(void)roi;
	(void)gray;
#endif
}

cv::Mat OpencvNativeRawF32Ops::convert(const cv::Mat& src)
{
#if MOSSE_USE_OPENCV
	cv::Mat cv8uc1 = cv::Mat::zeros(src.size(), CV_8UC1);
	for(int r = 0; r < src.rows; r++)
		for(int c = 0; c < src.cols; c++)
		{
			float val = src.at<float>(r,c);
			val = val > 255 ? 255 : val;
			cv8uc1.at<uchar>(r,c) = (unsigned char)val;
		}
	return cv8uc1;
#else
	(void)src;

	return {};
#endif
}

cv::Mat OpencvNativeRawF32Ops::real(cv::Mat image)
{
#if MOSSE_USE_OPENCV
	std::vector<cv::Mat> mats;
	cv::split(image, mats);
	return mats[0];
#else
	(void)image;
	return {};
#endif
}

cv::Mat OpencvNativeRawF32Ops::imag(cv::Mat image)
{
#if MOSSE_USE_OPENCV
	std::vector<cv::Mat> mats;
	cv::split(image, mats);
	return mats[1];
#else
	(void)image;
	return {};
#endif
}

cv::Mat OpencvNativeRawF32Ops::complexDivision(cv::Mat a, cv::Mat b)
{
#if MOSSE_USE_OPENCV
	std::vector<cv::Mat> pa;
	std::vector<cv::Mat> pb;

	cv::split(a, pa);
	cv::split(b, pb);

	cv::Mat divisior = 1.0 / (pb[0].mul(pb[0]) + pb[1].mul(pb[1]));

	std::vector<cv::Mat> pres;

	pres.push_back((pa[0].mul(pb[0]) + pa[1].mul(pb[1])).mul(divisior));
	pres.push_back((pa[1].mul(pb[0]) + pa[0].mul(pb[1])).mul(divisior));

	cv::Mat res;
	cv::merge(pres, res);
	return res;
#else
	(void)a;
	(void)b;

	return {};
#endif
}

cv::Rect OpencvNativeRawF32Ops::update(const cv::Mat& gray)
{
#if MOSSE_USE_OPENCV
	fi = imcrop(_roi, gray);
	//cv::resize(_roi, gray);
	cv::resize(fi, fi, init_sz);
	fi = preprocess(fi);

	Hi = complexDivision(Ai, Bi);
	cv::Mat response = fft(complexMultiplication(Hi, fft(fi)), true);

//	cv::normalize(response, response, 0, 1, cv::NORM_MINMAX);

	response *= 255.0;

	cv::Mat resp = real(response);
	cv::Mat resp_cv8u = cv::Mat_<unsigned char>(resp);

	cv::Point ps;
	cv::minMaxLoc(resp_cv8u, NULL, NULL, NULL, &ps);

	float dx = ps.x - init_sz.width / 2;
	float dy = ps.y - init_sz.height / 2;

	_roi = cv::Rect(_roi.x + dx, _roi.y + dy, init_sz.width, init_sz.height);
	auto psr = calculatePsr(response);  // TODO: BUG! The PSR must be calculated using the new peak, while this one uses the previous one

	train(gray);

	return _roi;
#else
	(void)gray;
	return{};
#endif
}

void OpencvNativeRawF32Ops::train(const cv::Mat& image)
{
#if MOSSE_USE_OPENCV
	fi = imcrop(_roi, image);
	cv::resize(fi, fi, init_sz);
	fi = preprocess(fi);

	fi_fft = fft(fi);
	Ai = _eta * complexMultiplication(gauss_fft, conj(fi_fft)) + (1 - _eta) * Ai;
	Bi = _eta * complexMultiplication(fi_fft, conj(fi_fft)) + (1 - _eta) * Bi;
#else
	(void)image;
#endif
}

}  // namespace Ut
}  // namespace Mosse
