#include <opencv2/core/utils/logger.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <algorithm>
#include <iostream>
using namespace cv;
using namespace std;

Mat img, tmp, dst;
int i, j, k, p, q;
float box[50][50];
void my_average_filter1(const Mat& pic, int side_length = 3)
{
	///**@side_length是盒的边长，应为奇数,未实现*/
	///1-不实现边界处理，2-边界填充0，3-边界填充最近值
	for (i = 1; i < pic.rows - 1; ++i)
	{
		for (j = 1; j < pic.cols - 1; ++j)
		{
			for (k = 0; k < 3; ++k)
			{
				dst.at<Vec3f>(i, j)[k] = (pic.at<Vec3f>(i - 1, j - 1)[k] + pic.at<Vec3f>(i - 1, j)[k] + pic.at<Vec3f>(i - 1, j + 1)[k] +
					pic.at<Vec3f>(i, j - 1)[k] + pic.at<Vec3f>(i, j)[k] + pic.at<Vec3f>(i, j + 1)[k] +
					pic.at<Vec3f>(i + 1, j - 1)[k] + pic.at<Vec3f>(i + 1, j)[k] + pic.at<Vec3f>(i + 1, j + 1)[k]) / 9;
			}
		}
	}

	for (i = 0; i < pic.rows; ++i) {
		for (k = 0; k < 3; ++k)
		{
			dst.at<Vec3f>(i, 0)[k] = pic.at<Vec3f>(i, 0)[k];
			dst.at<Vec3f>(i, pic.cols - 1)[k] = pic.at<Vec3f>(i, pic.cols - 1)[k];
		}

	}
	for (j = 0; j < pic.cols; ++j) {
		for (k = 0; k < 3; ++k)
		{
			dst.at<Vec3f>(0, j)[k] = pic.at<Vec3f>(0, j)[k];
			dst.at<Vec3f>(pic.rows - 1, i)[k] = pic.at<Vec3f>(pic.rows - 1, i)[k];
		}

	}
}

void my_median_filter()
{


}

void my_bilateral_filter(const Mat& pic, const int x, const double sigma)
{
	//sigma越大高斯函数图像越平，图像越模糊

	if (x % 2 == 0) return;
	//生成卷积核---------------------------------------
	int mid = (x + 1) / 2;
	double sum = 0;
	for (i = 1; i <= x; ++i)
	{
		for (j = 1; j <= x; ++j)
		{
			box[i][j] = exp(-((i - mid) * (i - mid) + (j - mid) * (j - mid)) / (2 * sigma * sigma));
			sum += box[i][j];
		}
	}
	for (i = 1; i <= x; ++i)
	{
		for (j = 1; j <= x; ++j)
		{
			box[i][j] /= sum;
		}
	}
	cout << "box\n";
	for (i = 1; i <= x; ++i)
	{
		for (j = 1; j <= x; ++j)
		{
			cout << box[i][j] << " ";
		}
		cout << "\n";
	}
	//--------------------------------------------

	//不进行边缘处理
	for (i = mid - 1; i <= pic.rows - mid; ++i)
	{
		for (j = mid - 1; j <= pic.cols - mid; ++j)
		{
			for (k = 0; k < 3; ++k)
			{
				dst.at<Vec3f>(i, j)[k]=0;
				for(p=1;p<=x;++p)
				{
					for(q=1;q<=x;++q)
					{
						dst.at<Vec3f>(i, j)[k]+=pic.at<Vec3f>(i-mid+p,j-mid+q)[k]*box[p][q];
					}
				}
			}
		}
	}



}


int main()
{
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);
	img = imread("2.png", IMREAD_ANYCOLOR | IMREAD_ANYDEPTH);
	if (!img.data)
	{
		cout << "error.\n";
		system("pause");
	}
	resize(img, img, Size(1280, 720), 0, 0, INTER_AREA);
	cout << "图像属性\n";
	cout << "img.rows=" << img.rows << endl;
	cout << "img.cols=" << img.cols << endl;
	cout << "img.type()=" << img.type() << endl;
	cout << "img.channels()=" << img.channels() << endl;

	dst = Mat::zeros(img.rows, img.cols, img.type());
	img.convertTo(img, CV_32F, 1.0 / 255);
	dst.convertTo(dst, CV_32F, 1.0 / 255);

	imshow("原图", img);

	my_average_filter1(img, 3);
	imshow("均值滤波图", dst);

	//img.convertTo(img, CV_8U, 255);
	//dst.convertTo(dst, CV_8U, 255);
	medianBlur(img, dst, 3);
	imshow("中值滤波图", dst);

	my_bilateral_filter(img, 5, 1.5);
	//bilateralFilter(img,dst,15,150,3);
	//GaussianBlur()
	imshow("高斯双边滤波",dst);
	waitKey(0);

}