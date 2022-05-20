#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
using namespace std;
using namespace cv;

int main()
{
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);
	Mat img1 = imread("2.png");
	Mat img2 = imread("00000001.jpg");
	Mat dst;
	cout << "img1.size()=" << img1.size() << endl;
	cout << "img2.size()=" << img2.size() << endl;
	resize(img1,img1,Size(1920,1080),0,0,INTER_AREA);
	resize(img2,img2,Size(1920,1080),0,0,INTER_AREA);
	cout << "img1.size()=" << img1.size() << endl;
	cout << "img2.size()=" << img2.size() << endl;

	addWeighted(img1, 0.5, img2, 1-0.5, 0.0, dst);
	if (!dst.data)
	{
		cout << "error.";
		system("pause");
		return -1;
	}
	namedWindow("pic");
	namedWindow("img1");
	namedWindow("img2");
	imshow("pic", dst);
	imshow("img1", img1);
	imshow("img2", img2);
	waitKey(0);
}
