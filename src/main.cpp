#include "tracker.h"
#include <assert.h>
#include "selectROI.h"
#include <Mosse.hpp>
#include <Port/Thread/StlThread.hpp>
#include <Port/Thread/PthreadThread.hpp>
#include <Port/MossePort.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio/legacy/constants_c.h>

Mosse::Tracker *sTracker;
constexpr static bool sUsePredefinedRoi = true;
const std::array<cv::Rect, 2> sPredefinedRoiArr {{
	{273, 141, 74, 78},
	{182, 144, 81, 84}
}};
const cv::Rect &sPredefinedRoi = sPredefinedRoiArr[1];

static cv::Mat bgr2gray(const cv::Mat& image)
{
	cv::Mat res;
	int chans = image.channels();
	if(chans == 3)
		cv::cvtColor(image, res, cv::ColorConversionCodes::COLOR_BGR2GRAY);
	else res = image.clone();
	return res;
}

void runPort()
{
	cv::VideoCapture cap;
	cap.open("save.avi");

	mosseTracker track;
	selectROI box;
	box.init_param();
	cv::Mat frame;
	bool init = true;
	cv::Rect roi;
	std::string trackingWindow = "tracking.jpg";
	while(cap.read(frame))
	{
		auto gray = bgr2gray(frame);
		if(init)
		{
			if (sUsePredefinedRoi) {
				roi = sPredefinedRoi;
			} else {
				roi = box.add(trackingWindow, frame);
			}
			Mosse::Tp::Roi mosseRoi{{roi.y, roi.x}, {roi.size().height, roi.size().width}};
			Mosse::Tp::Image mosseImage{gray.data, gray.size().height, gray.size().width};
			sTracker->init(mosseImage, mosseRoi);
			init = false;
		}
		else {
			Mosse::Tp::Image mosseImage{gray.data, gray.size().height, gray.size().width};
			sTracker->update(mosseImage, true);
			ohdebug(main, sTracker->lastPsr());
			auto mosseRoi = sTracker->roi();
			roi = {mosseRoi.origin(1), mosseRoi.origin(0), mosseRoi.size(1), mosseRoi.size(0)};  // Create cv-compatible row-major ROI
		}
		cv::rectangle(frame, roi, cv::Scalar(255,255,0));
		cv::imshow(trackingWindow, frame);
		cv::waitKey(20);
	}
	cap.release();
	box.exit();
}

void run()
{
	cv::VideoCapture cap;
	cap.open("save.avi");

	mosseTracker track;
	selectROI box;
	box.init_param();
	cv::Mat frame;
	bool init = true;
	cv::Rect roi;
	std::string trackingWindow = "tracking.jpg";
	while(cap.read(frame))
	{
		auto gray = bgr2gray(frame);
		if(init)
		{
			if (sUsePredefinedRoi) {
				roi = sPredefinedRoi;
			} else {
				roi = box.add(trackingWindow, frame);
			}
			track.init(roi, gray);
			init = false;
		}
		else 
			roi = track.update(gray);
		cv::rectangle(frame, roi, cv::Scalar(255,255,0));
		cv::imshow(trackingWindow, frame);
		cv::waitKey(20);
	}
	cap.release();
	box.exit();
}

void img2avi(char* address)
{
	assert(address != NULL);
	char buf[128];
	int nums = 1;
	sprintf(buf, "%s/%04d.jpg", address, nums);

	cv::Mat frame = cv::imread(buf);

	while(frame.data == NULL)
	{
		nums++;
		printf("num = %d\n", nums);
		sprintf(buf, "%s/%04d.jpg", address, nums);
		frame = cv::imread(buf);
	}

	cv::VideoWriter save;
	save.open("save.avi", CV_FOURCC('M','P','4','2'), 25.0, frame.size(), true);
	save << frame;
	nums++;

	while(frame.data != NULL)
	{
		sprintf(buf, "%s/%04d.jpg", address, nums++);
		frame = cv::imread(buf);
		save << frame;
	}
	save.release();
	printf("save over!\n");
}

int main()
{
//	Mosse::Port::StlThread stlThread;
	Mosse::Port::PthreadThread pthreadThread;
	//img2avi((char*)"img");
//	sTracker = &Mosse::getDebugStub();
//	sTracker = &Mosse::getNaive();
//	sTracker = &Mosse::getNaiveMultithreaded(pthreadThread, 2);
//	sTracker = &Mosse::getNaiveRadix2Fft();
	sTracker = &Mosse::getFp16AbRawF32BufDynAlloc();
	runPort();
//	run();
	debug(MallocCounter::getPeakCount());
	return 0;
}
