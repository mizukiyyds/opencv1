#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <Windows.h>
#include <cstdio>
#include <algorithm>
using namespace cv;
using namespace std;

int i, j, k, p, q;
Mat img, dst;


//膨胀用来处理缺陷问题，腐蚀就是把毛刺给腐蚀掉了
void my_dilate(const Mat& pic, Mat& dst, const int odd_length, const int times = 1)
{
	Mat tmp1 = pic.clone();
	Mat tmp2 = pic.clone();
	int maxx = -1;
	int mid = (odd_length + 1) / 2;
	for (k = 1; k <= times; ++k)
	{
		for (i = mid - 1; i < pic.rows - mid; ++i)
		{
			for (j = mid - 1; j < pic.cols - mid; ++j)
			{
				maxx = -1;
				for (p = i - mid + 1; p <= i + mid - 1; ++p)
				{
					for (q = j - mid + 1; q <= j + mid - 1; ++q)
					{
						maxx = max(tmp1.at<uchar>(i, j), tmp1.at<uchar>(p, q));

					}
				}
				tmp2.at<uchar>(i, j) = maxx;
			}
		}
		tmp1 = tmp2.clone();
		// for (i = 0; i < pic.rows; ++i)
		// {
		// 	dst.at<uchar>(i, 0) = pic.at<uchar>(i, 0);
		// 	dst.at<uchar>(i, pic.cols - 1) = pic.at<uchar>(i, pic.cols - 1);
		// }
		// for (j = 0; j < pic.cols; ++j)
		// {
		// 	dst.at<uchar>(0, j) = pic.at<uchar>(0, j);
		// 	dst.at<uchar>(pic.rows - 1, j) = pic.at<uchar>(pic.rows - 1, j);
		// }
	}
	dst = tmp2;
}


//腐蚀
void my_erode(const Mat& pic, Mat& dst, const int odd_length, const int times = 1)
{
	Mat tmp1 = pic.clone();
	Mat tmp2 = pic.clone();
	int minn = 256;
	int mid = (odd_length + 1) / 2;
	for (k = 1; k <= times; ++k)
	{
		for (i = mid - 1; i < pic.rows - mid; ++i)
		{
			for (j = mid - 1; j < pic.cols - mid; ++j)
			{
				minn = -1;
				for (p = i - mid + 1; p <= i + mid - 1; ++p)
				{
					for (q = j - mid + 1; q <= j + mid - 1; ++q)
					{
						minn = min(tmp1.at<uchar>(i, j), tmp1.at<uchar>(p, q));

					}
				}
				tmp2.at<uchar>(i, j) = minn;
			}
		}
		tmp1 = tmp2.clone();

	}
	dst = tmp2;
}

void my_open(const Mat& pic, Mat& dst, const int odd_length, const int times = 1)
{

	my_erode(pic, dst, odd_length, times);
	my_dilate(dst, dst, odd_length, times);

}
void my_close(const Mat& pic, Mat& dst, const int odd_length, const int times = 1)
{

	my_dilate(pic, dst, odd_length, times);
	my_erode(dst, dst, odd_length, times);
}

void my_gradient(const Mat& pic, Mat& dst, const int odd_length, const int times = 1)
{
	Mat tmp1 = Mat::zeros(pic.rows, pic.cols, pic.type());
	my_dilate(pic, dst, odd_length, times);
	my_erode(pic, tmp1, odd_length, times);
	dst -= tmp1;
}

/**高帽运算，显示毛刺信息
@param odd_length 卷积核长度，>=3的奇数
*/
void my_tophat(const Mat& pic, Mat& dst, const int odd_length, const int times = 1)
{
	Mat tmp1 = Mat::zeros(pic.rows, pic.cols, pic.type());
	my_open(pic, tmp1, odd_length, times);
	dst = pic - tmp1;

}
void my_blackhat(const Mat& pic, Mat& dst, const int odd_length, const int times = 1)
{
	Mat tmp1 = Mat::zeros(pic.rows, pic.cols, pic.type());
	my_close(pic, tmp1, odd_length, times);
	dst = tmp1 - pic;

}

int main()
{
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);
	img = imread("1.png",IMREAD_GRAYSCALE);
	if (!img.data)
	{
		cout << "error";
		system("pause");
		return -1;
	}
	resize(img, img, Size(128, 72));
	//imshow("img", img);
	//cvtColor(img, img, COLOR_BGR2GRAY);
	cout << "图像属性\n";
	cout << "img.rows=" << img.rows << endl;
	cout << "img.cols=" << img.cols << endl;
	cout << "img.type()=" << img.type() << endl;
	cout << "img.channels()=" << img.channels() << endl;

	dst = Mat::zeros(img.rows, img.cols, img.type());
	//dst.create(img.rows, img.cols, img.type();
	//cout<<(int)dst.at<uchar>(0,0);   create创造的默认值为205，zeros创造的默认值为0

	//my_close(img,dst,3,1);
	Mat str = getStructuringElement(MORPH_CROSS, Size(3, 3));
	morphologyEx(img, dst, MORPH_GRADIENT, str, Point(-1, -1), 1);
	//morphologyEx(img, dst, MORPH_CLOSE, str, Point(-1, -1), 1);

	resize(dst, dst, Size(480, 320));
	//imshow("api-dst", dst);
	//my_gradient(img, dst, 3, 2);
	//my_gradient(img, dst, 3, 2);
	imshow("dst", dst);
	waitKey(0);
	imwrite("yzm.png",dst);
	system("pause");
}
