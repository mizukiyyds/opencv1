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
	///**@side_length�Ǻеı߳���ӦΪ����,δʵ��*/
	///1-��ʵ�ֱ߽紦��2-�߽����0��3-�߽�������ֵ
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
	//sigmaԽ���˹����ͼ��Խƽ��ͼ��Խģ��

	if (x % 2 == 0) return;
	//���ɾ����---------------------------------------
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

	//�����б�Ե����
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
	cout << "ͼ������\n";
	cout << "img.rows=" << img.rows << endl;
	cout << "img.cols=" << img.cols << endl;
	cout << "img.type()=" << img.type() << endl;
	cout << "img.channels()=" << img.channels() << endl;

	dst = Mat::zeros(img.rows, img.cols, img.type());
	img.convertTo(img, CV_32F, 1.0 / 255);
	dst.convertTo(dst, CV_32F, 1.0 / 255);

	imshow("ԭͼ", img);

	my_average_filter1(img, 3);
	imshow("��ֵ�˲�ͼ", dst);

	//img.convertTo(img, CV_8U, 255);
	//dst.convertTo(dst, CV_8U, 255);
	medianBlur(img, dst, 3);
	imshow("��ֵ�˲�ͼ", dst);

	my_bilateral_filter(img, 5, 1.5);
	//bilateralFilter(img,dst,15,150,3);
	//GaussianBlur()
	imshow("��˹˫���˲�",dst);
	waitKey(0);

}