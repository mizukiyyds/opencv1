#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <Windows.h>
using namespace std;
using namespace cv;
Mat img,pic;
int i, j, k;
int alpha = 1000, beta = 500;
int cal_alpha,cal_beta;
int x = 0, y = 0, z = 0;
int flag;
void CorrectPath(char* path_s) {  //校准路径，
	char path[2001] = "";
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
	strcpy_s(path_s, 2000, path);
}
char path[1001];
auto on_Trackbar(int, void*)
{
	if(alpha==0) alpha=1;
	//img.at<Vec3f>(i,j)[0]=alpha*saturate_cast<Vec3f>(i,j)[0]+beta;


}

int main()
{
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);
	cout << "输入文件名：";
	gets_s(path, 1000);
	pic = imread(path, IMREAD_ANYCOLOR | IMREAD_ANYDEPTH);
	if (pic.empty())
	{
		cout << "加载图像出错.";
		system("pause");
		return -1;
	}
	resize(pic, pic, Size(1280, 720), 0, 0, INTER_AREA);
	pic.convertTo(pic, CV_32F, 1.0 / 255);
	cout << "图像属性\n";
	cout << "img.rows=" << pic.rows << endl;
	cout << "img.cols=" << pic.cols << endl;
	cout << "img.type()=" << pic.type()<<endl;
	cout << "img.channels()=" << pic.channels()<<endl;
	//Mat fimg;
	namedWindow("pic");
	createTrackbar("alpha", "pic", &alpha, 2000, on_Trackbar);
	createTrackbar("beta", "pic", &beta, 1000, on_Trackbar);
	while (1)
	{
		cal_alpha=alpha;
		cal_beta=beta-500;
		//cin
		for (i = 0; i < pic.rows; ++i)
		{
			for (j = 0; j < pic.cols; ++j)
			{
				pic.at<Vec3f>(i, j)[0] *= cal_alpha / 1000.0;
				pic.at<Vec3f>(i, j)[1] *= cal_alpha / 1000.0;
				pic.at<Vec3f>(i, j)[2] *= cal_alpha / 1000.0;
				pic.at<Vec3f>(i, j)[0] += cal_beta / 1000.0;
				pic.at<Vec3f>(i, j)[1] += cal_beta / 1000.0;
				pic.at<Vec3f>(i, j)[2] += cal_beta / 1000.0;
			}
		}
		imshow("pic", pic);
		flag=waitKey(100);
		if(flag=='b') break;
		else if(flag=='s')
		{
			pic.convertTo(pic,CV_8U,255);
			imwrite("output.jpg",pic);
			pic.convertTo(pic,CV_32F,1.0/255);
		}
		for (i = 0; i < pic.rows; ++i)
		{
			for (j = 0; j < pic.cols; ++j)
			{
				pic.at<Vec3f>(i, j)[0] -= cal_beta / 1000.0;
				pic.at<Vec3f>(i, j)[1] -= cal_beta / 1000.0;
				pic.at<Vec3f>(i, j)[2] -= cal_beta / 1000.0;
				pic.at<Vec3f>(i, j)[0] /= cal_alpha / 1000.0;
				pic.at<Vec3f>(i, j)[1] /= cal_alpha / 1000.0;
				pic.at<Vec3f>(i, j)[2] /= cal_alpha / 1000.0;
				
			}
		}
		
	}
	
	cvtColor(pic, pic, COLOR_BGR2HSV);



}
