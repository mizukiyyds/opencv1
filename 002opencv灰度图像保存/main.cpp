#include<opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

using namespace cv;

int main() {
	
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);
	printf("version: OpenCV " CV_VERSION);
	Mat img = imread("2.png",IMREAD_GRAYSCALE);
	namedWindow("第一张图片", WINDOW_FREERATIO);
	imshow("第一张图片", img);
	imwrite("123.png",img);
	waitKey();
	return 0;
}
