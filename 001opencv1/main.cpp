#include<opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

using namespace cv;

int main() {
	cv::utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);
	printf("version: OpenCV " CV_VERSION);
	Mat img = imread("雪糕拉菲.mp4");
	namedWindow("第一张图片",WINDOW_FREERATIO);
	imshow("第一张图片", img);
	waitKey();
	return 0;
}