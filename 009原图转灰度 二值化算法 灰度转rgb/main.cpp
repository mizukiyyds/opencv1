#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <algorithm>
using namespace cv;
using namespace std;

Mat img, dst;
int i, j, k;
char path[1001];

int flag;
//校准路径
void CorrectPath(char* path_s) {
	char path[1001] = "";
	int l = (int)strlen(path_s);
	int cnt = 0;
	for (i = 0; i < l; ++i) {
		if (path_s[i] == '\\') {
			path[i + cnt] = path_s[i];
			++cnt;
			path[i + cnt] = '\\';
		}
		else {
			path[i + cnt] = path_s[i];
		}
	}
	strcpy_s(path_s, 1000, path);
}


void my_BGR2GRAY(const Mat& pic, Mat& dst)
{
	Mat tmp = Mat::zeros(pic.rows, pic.cols, CV_8U);
	for (i = 0; i < pic.rows; ++i)
	{
		for (j = 0; j < pic.cols; ++j)
		{
			tmp.at<uchar>(i, j) =saturate_cast<uchar>( (pic.at<Vec3f>(i, j)[0] * 0.11 + pic.at<Vec3f>(i, j)[1] * 0.59 + pic.at<Vec3f>(i, j)[2] * 0.3) * 255);
			//cout<<(float)dst.at<float>(i,j)<<endl;
		}
	}
	tmp.copyTo(dst);
}

// void my_GRAY2BGR(const Mat& pic, Mat& dst)
// {
// 	for (i = 0; i < pic.rows; ++i)
// 	{
// 		for (j = 0; j < pic.cols; ++j)
// 		{
//
// 			 dst.at<Vec3f>(i, j)[0] = pic.at<uchar>(i, j)/255.0/3;
// 			 dst.at<Vec3f>(i, j)[1] = pic.at<uchar>(i, j)/255.0/3;
// 			 dst.at<Vec3f>(i, j)[2] = pic.at<uchar>(i, j)/255.0/3;
//
// 		}
// 	}
//
// }

void showinfo(const Mat& pic)
{
	cout << "图像属性\n";
	cout << "img.rows=" << pic.rows << endl;
	cout << "img.cols=" << pic.cols << endl;
	cout << "img.type()=" << pic.type() << endl;
	cout << "img.channels()=" << pic.channels() << endl;

}

//二值化，pic应为灰度图像
int my_otsu(const Mat& pic, Mat& dst)
{
	double p[256] = {};               //各种灰度的出现概率
	double sump[256] = {};            //概率前缀和
	double avg[256] = {};             //带权前缀和
	double sum = pic.rows * pic.cols;   //点数
	double var = 0, tmp = 0;                   //类间方差
	int flag = 0;
	for (i = 0; i < pic.rows; ++i)
	{
		const uchar* ptr = pic.ptr<uchar>(i);
		for (j = 0; j < pic.cols; ++j)
		{
			++p[ptr[j]];            //统计灰度直方图
		}
	}
	for (i = 0; i < 256; ++i) p[i] /= sum;
	sump[0] = p[0];
	for (i = 1; i < 256; ++i) sump[i] = sump[i - 1] + p[i];
	for (i = 1; i < 256; ++i) avg[i] = avg[i - 1] + i * p[i];
	for (k = 0; k < 256; ++k)
	{
		tmp = (avg[255] * sump[k] - avg[k]) * (avg[255] * sump[k] - avg[k]) / (sump[k] * (1 - sump[k]));
		if (tmp > var) {
			var = tmp;
			flag = k;
		}
	}
	pic.copyTo(dst);
	for (i = 0; i < pic.rows; ++i)
	{
		uchar* ptr = dst.ptr<uchar>(i);
		for (j = 0; j < pic.cols; ++j)
		{
			if (ptr[j] > flag) ptr[j] = 255;
			else ptr[j] = 0;
		}
	}
	return flag;
}

int my_maxentropy(const Mat& pic, Mat& dst)
{
	double p[256] = {};
	double sump[256] = {};
	double var = -0xfffffff,tmp=0;				//熵
	int flag = 0;
	double sum = pic.rows * pic.cols;   //点数
	for (i = 0; i < pic.rows; ++i)
	{
		const uchar* ptr = pic.ptr<uchar>(i);
		for (j = 0; j < pic.cols; ++j)
		{
			++p[ptr[j]];            //统计灰度直方图
		}
	}
	for (i = 0; i < 256; ++i) p[i] /= sum;
	sump[0] = p[0];
	for (i = 1; i < 256; ++i) sump[i] = sump[i - 1] + p[i];
	for (k = 0; k < 256; ++k)
	{
		tmp=0;
		for (i = 0; i <= k; ++i)
		{
			tmp -= log(0.00000001 + p[i] / sump[k]) * p[i] / sump[k];		//防止log(0)
		}
		for (i; i < 256; ++i)
		{
			tmp -= log(0.00000001 + p[i] /(1- sump[k])) * p[i] / (1-sump[k]);		//防止log(0)

		}
		if(tmp>var)
		{
			var=tmp;
			flag=k;
		}
	}
	pic.copyTo(dst);
	for (i = 0; i < pic.rows; ++i)
	{
		uchar* ptr = dst.ptr<uchar>(i);
		for (j = 0; j < pic.cols; ++j)
		{
			if (ptr[j] > flag) ptr[j] = 255;
			else ptr[j] = 0;
		}
	}
	return flag;
}


void my_AdaptiveThreshold(Mat& pic, Mat& dst, uchar Maxval, int Subsize, double c=0){
 
	if (pic.channels() > 1) cvtColor(pic, pic, COLOR_BGR2GRAY);
	Mat smooth;
	GaussianBlur(pic, smooth, cv::Size(Subsize, Subsize),0,0); //高斯滤波
	smooth = smooth - c;
	//阈值处理
	pic.copyTo(dst);
	for (int r = 0; r < pic.rows;++r){
		const uchar* picptr = pic.ptr<uchar>(r);
		const uchar* smoothptr = smooth.ptr<uchar>(r);
		uchar* dstptr = dst.ptr<uchar>(r);
		for (int c = 0; c < pic.cols; ++c){
			if (picptr[c]>smoothptr[c]){
				dstptr[c] = Maxval;
			}
			else
				dstptr[c] = 0;
		}
	}
 
}


int main()
{
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);
	cout << "输入文件名：";
	gets_s(path, 1000);
	CorrectPath(path);
	img = imread(path);
	if (img.empty())
	{
		return -1;
	}

	resize(img, img, Size(1280, 720));
	img.convertTo(img, CV_32F, 1.0 / 255);
	showinfo(img);
	my_BGR2GRAY(img, img);
	imshow("img", img);
	//cout << "value:" << my_maxentropy(img, dst) << endl;
	//threshold(img,dst,120,255,THRESH_OTSU);
	my_AdaptiveThreshold(img,dst,255,3,0);
	imshow("dst", dst);
	imwrite("myadaptive.png", dst);
	waitKey(0);
}