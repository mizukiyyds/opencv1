#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
using namespace cv;
using namespace std;
int i, j, k;
int dx;int dy;
Mat img, dst,tmp;//1920*1080
float alpha, beta;
int rnd(float a){
    int b;
    if(a > 0){
        b = (a*2+1)/2;
    }else{
        b = (a*2-1)/2;  
    }
    return b;
}


int main()
{
	cout << "dx:"; cin >> dx;
	cout << "dy:"; cin >> dy;
	img = imread("2.png");
	if (img.empty())
	{
		cout << "error.";
		system("pause");
		return -1;
	}
	img.convertTo(img,CV_32F,1.0/255,0);
	dst = Mat::zeros(dy, dx, img.type());
	alpha = (float)img.rows / dy;
	beta = (float)img.cols / dx;
	if (img.channels() == 1)
	{
		for (i = 0; i < dy; ++i)
		{
			for (j = 0; j < dx; ++j)
			{
				dst.at<uchar>(i, j) = img.at<uchar>(i * alpha, j * beta);
				
			}
		}
	}
	else if (img.channels() == 3)
	{
		for (i = 0; i < dy; ++i)
		{
			for (j = 0; j < dx; ++j)
			{
				/*dst.at<Vec3f>(i, j)[0] = img.at<Vec3f>(i,j)[0];
				dst.at<Vec3f>(i, j)[1] = img.at<Vec3f>(i,j)[1];
				dst.at<Vec3f>(i, j)[2] = img.at<Vec3f>(i,j)[2];*/
				dst.at<Vec3f>(i, j)[0] = img.at<Vec3f>(rnd(i * alpha),rnd( j * beta))[0];
				dst.at<Vec3f>(i, j)[1] = img.at<Vec3f>(rnd(i * alpha),rnd( j * beta))[1];
				dst.at<Vec3f>(i, j)[2] = img.at<Vec3f>(rnd(i * alpha),rnd( j * beta))[2];
			}
		}
	}
	imshow("dst",dst);
	waitKey(0);
}
