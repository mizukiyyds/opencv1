#include <opencv2/core/utils/logger.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>
using namespace std;
using namespace cv;
int i, j, k,p;
Mat img, dst;
double q;
//
int thre;
int* *cnt;
int maxx=-1;
int sita;
int pr = 0;
int lim;     //pr极大值
int posx,posy;
const double tohu=3.1415926536/180.0;
//

int rnd(double val){
    int b;
    if(val > 0){
        b = (val*2+1)/2;
    }else{
        b = (val*2-1)/2;  
    }
    return b;
}


void onchange(int pos, void* userdata)
{
	Mat *ptr=static_cast<Mat*>(userdata);
	Mat tmp=(*ptr).clone();
	int y=0;
	for(k=maxx;k>=pos;--k)
	{
		for(i=0;i<=90;++i)
		{
			for(j=0;j<lim;++j)
			{
				if(cnt[i][j]==k)
				{
					for(p=0;p<tmp.rows;++p)
					{
						y=rnd(-cos(tohu*i)*p/sin(tohu*i)+j/sin(tohu*i));
						if(y>=0&&y<tmp.cols)
						{
							tmp.at<uchar>(p,y)=255;
						}
					}
				}
			}
		}
	}
	tmp.copyTo(*ptr);
	imshow("dst",*ptr);
}

void my_huofu(const Mat& pic, Mat& dst, double ctrl)
{
	dst=Mat::zeros(pic.rows,pic.cols,CV_8UC1);
	double pmax = 0;		//pr的最大值，辅助角公式
	maxx=-1;				//cnt的最大值

	//初始化二维数组，计数
	lim=rnd(sqrt(pic.rows*pic.rows+pic.cols*pic.cols))+1;
	cnt = new int* [91];    //90度
	for (i = 0; i <= 90; ++i)
	{
		cnt[i] = new int[lim]();
	}
	//

	for (i = 0; i < pic.rows; ++i)
	{
		const unsigned char* ptrpic = pic.ptr<uchar>(i);
		for (j = 0; j < pic.cols; ++j)
		{
			if (ptrpic[j] != 0)
			{
				pmax=i+j;//sqrt(i*i+j*j);
				for (sita = 0; sita <= 90; ++sita)     //枚举，描点
				{
					for (pr = 0; pr <= pmax; ++pr)
					{
						if (abs(pr - i * cos(tohu*sita) - j * sin(tohu*sita)) <= ctrl){
							++cnt[sita][pr];
						}
					}
				}
			}
		}
	}
	for(i=0;i<=90;++i)
	{
		for(j=0;j<lim;++j)
		{
			if(cnt[i][j]>maxx)
			{
				maxx=cnt[i][j];
			}
		}
	}
	thre=maxx;
	createTrackbar("阈值","dst",&thre,maxx,onchange,const_cast<Mat*>(&dst));
}

void my_huofucircle(const Mat& pic,Mat &dst,int threshold=130)
{
	int r=0,p=0,q=0;               //循环变量，实心圆
	int lx=0,rx=0,uy=0,dy=0;
	int** cnt=new int*[pic.rows];
	for(i=0;i<pic.rows;++i)
	{
		cnt[i]=new int[pic.cols];
	}
	for(i=0;i<pic.rows;++i)
	{
		const uchar* ptr=pic.ptr<uchar>(i);
		for(j=0;j<pic.cols;++j)
		{
			if(!ptr[j]) continue;
			++cnt[i][j];
			for(r=1;r<=100;++r)
			{
				lx=max(0,i-r);
				rx=min(pic.cols,i+r);

				//for(p=i;p<=)
			}
		}
	}




	for(i=0;i<pic.rows;++i)
	{
		delete[] cnt[i];
	}
	delete[] cnt;
}

int main()
{
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);
	img = imread("2.png", IMREAD_GRAYSCALE);
	if (img.empty())
	{
		return -1;
	}
	resize(img,img,Size(720,480));
	dst=Mat::zeros(img.rows,img.cols,CV_8UC3);
	GaussianBlur(img, img, Size(5, 5), 0.4);
	Canny(img, img, 60, 120);
	imshow("img",img);
	waitKey(0);
	namedWindow("dst");
	//--------------------------
	vector<Vec4i> lines;
	HoughLinesP(img,lines,1,CV_PI/180,150,20,5);
	for(i=0;i < lines.size();++i)
	{
		line(dst,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(23,180,55),1);
	}
	//-----------------------------
	//my_huofu(img,dst,1);
	imshow("dst", dst);
	waitKey(0);
	system("pause");

}