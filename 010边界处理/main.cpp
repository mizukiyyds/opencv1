#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
using namespace cv;
using namespace std;

Mat img, dst;

int i, j, k;

enum my_types
{
#define border_constant 1
#define border_replicate 2
};

/**
 *@param pic 原图像需为单通道灰度图像或者3通道浮点型
 *@param direct 1↑ 2↓ 3← 4→
 *@param type see my_types
 */
void my_makeborder(const Mat& pic, Mat& dst, int len, int direct, int type, const Scalar& color = Scalar(0))
{
	Mat tmp;
	switch (direct)
	{
	case 1:
		tmp = Mat::zeros(pic.rows + len, pic.cols, pic.type());
		if (pic.channels() == 1)
		{
			for (i = 0; i <= len - 1; ++i)
			{
				unsigned char* ptrtmp = tmp.ptr<uchar>(i);
				if (type == border_constant)
				{
					for (j = 0; j < pic.cols; ++j)
					{
						ptrtmp[j] = color[0];
					}
				}
				else if (type == border_replicate)
				{
					const unsigned char* ptrpic = pic.ptr<uchar>(2 * len - i - 1);
					for (j = 0; j < pic.cols; ++j)
					{
						ptrtmp[j] = ptrpic[j];
					}
				}
				else
				{
					cout << "error";
					return;
				}
			}
			for (i; i <= pic.rows + len - 1; ++i)
			{
				unsigned char* ptrtmp = tmp.ptr<uchar>(i);
				const unsigned char* ptrpic = pic.ptr<uchar>(i - len);
				for (j = 0; j < pic.cols; ++j)
				{
					ptrtmp[j] = ptrpic[j];
				}
			}
		}
		else if (pic.channels() == 3)
		{
			tmp.convertTo(tmp, CV_32FC3, 1.0 / 255);
			for (i = 0; i <= len - 1; ++i)
			{
				Vec3f* ptrtmp = tmp.ptr<Vec3f>(i);
				if (type == border_constant)
				{
					for (j = 0; j < pic.cols; ++j)
					{
						ptrtmp[j][0] = color[0];
						ptrtmp[j][1] = color[1];
						ptrtmp[j][2] = color[2];
					}
				}
				else if (type == border_replicate)
				{
					const Vec3f* ptrpic = pic.ptr<Vec3f>(2 * len - i - 1);
					for (j = 0; j < pic.cols; ++j)
					{
						ptrtmp[j][0] = ptrpic[j][0];
						ptrtmp[j][1] = ptrpic[j][1];
						ptrtmp[j][2] = ptrpic[j][2];
					}
				}
				else
				{
					cout << "error";
					return;
				}
			}
			for (i; i <= pic.rows + len - 1; ++i)
			{
				Vec3f* ptrtmp = tmp.ptr<Vec3f>(i);
				const Vec3f* ptrpic = pic.ptr<Vec3f>(i - len);
				for (j = 0; j < pic.cols; ++j)
				{
					ptrtmp[j][0] = ptrpic[j][0];
					ptrtmp[j][1] = ptrpic[j][1];
					ptrtmp[j][2] = ptrpic[j][2];
				}
			}
		}
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		return;
	}
	tmp.copyTo(dst);
}

int main()
{
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);
	img = imread("2.png",IMREAD_GRAYSCALE);
	if (img.empty())
	{
		return -1;
	}
	resize(img,img,Size(1280,720));
	//img.convertTo(img,CV_32F,1.0/255);
	my_makeborder(img,dst,20,1,border_constant,Scalar(0,0,255));
	//copymakeborder();
	imshow("dst",dst);
	waitKey(0);
	img.convertTo(dst,CV_8U,255.0);
	imwrite("3channels-grayscale-dst.png",dst);

}