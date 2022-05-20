#include <opencv2/core/utils/logger.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int i,j,k;
int roix,roiy;
Mat img,logo,dst,roi;
void onMouse(int event, int x, int y, int flags, void* userdata)
{
	if(event==EVENT_LBUTTONDOWN)
	{
		roi=img(Rect(x,y,logo.cols,logo.rows));
		logo.convertTo(roi,roi.type());
		//logo.copyTo(roi);

		imshow("dst",img);
		waitKey(0);
	}

}

int main()
{
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);
	img=imread("2.png");
	logo=imread("1.jpg");
	namedWindow("dst");
	if(img.empty()||logo.empty())
	{
		return -1;
	}
	resize(img,img,Size(1280,720));
	resize(logo,logo,Size(300,150));
	setMouseCallback("dst",onMouse);
	imshow("dst",img);
	waitKey(0);
	//system("pause");


}
