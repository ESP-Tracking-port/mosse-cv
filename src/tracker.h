#ifndef _TRACKER_H_HH
#define _TRACKER_H_HH

#include <opencv2/opencv.hpp>

#if 1
# include <iostream>
# define debug(a) std::cout << (#a) << " : " << (a) << std::endl
# define debugstr(a) std::cout << (a) << std::endl;
#else
# define debug(...)
# define debugstr(...)
#endif

class mosseTracker
{
public:
	mosseTracker();
	virtual ~mosseTracker();
public:
	void init(cv::Rect roi, const cv::Mat& image);
	cv::Rect update(const cv::Mat& image);
protected:
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
