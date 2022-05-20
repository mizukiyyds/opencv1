#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
using namespace cv;
using namespace std;
int i, j, k;
Mat img, dst, kernel_border, kernel_sharp;
int main()
{
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);
	img = imread("2.png", IMREAD_GRAYSCALE);
	if (img.empty())
	{
		return -1;
	}
	resize(img, img, Size(1280, 720));
	GaussianBlur(img,img,Size(5,5),0.4);
	imshow("gb",img);
	Canny(img,dst,60,120,3);
	imshow("dst",dst);
	waitKey(0);
	// kernel_border = (Mat_<int>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
	 kernel_sharp = (Mat_<int>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	//
	 filter2D(img, dst, -1, kernel_sharp, Point(-1, -1), 0);
	imshow("dst", dst);
	waitKey(0);
	//kernel<<-1,0,1,-2,0,2,-1,0,1;
}