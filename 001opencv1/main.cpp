#include<opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

using namespace cv;

int main() {
	cv::utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);
	printf("version: OpenCV " CV_VERSION);
	Mat img = imread("ѩ������.mp4");
	namedWindow("��һ��ͼƬ",WINDOW_FREERATIO);
	imshow("��һ��ͼƬ", img);
	waitKey();
	return 0;
}