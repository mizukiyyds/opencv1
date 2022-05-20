#include <opencv2/core/utils/logger.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <algorithm>
using namespace std;
using namespace cv;
Mat img, bak, hdt, hdt2, edge, dst;
int i, j, k;
vector <Vec4i> orglines;
vector <Vec4i> lines;
vector <Vec4i>::iterator iter;
int lim;			//霍夫直线变换，点的个数
long long maxx = -1;			//2最远点的最大距离;
long long maxd;			//第三点到n1的距离
long long d;			//两点距离
const int paperweight = 420;
const int paperheight = 594;
const int sameptdis=20000;
struct mys
{
	int x;
	int y;
}pt[10001], n1, n2, n3, n4;		//4个角点
int f1, f2, f3, f4;					//flag
double mincos = 2, tmpcos, tmpcos2;
long long d1, d2, d3;			//n3->n1,n2的距离，用于余弦定理.int 在计算sqrt(d1*d2)时会溢出
long long disto3;			//n1->n3的距离
int condition;		//四边形状态，以最长边为分界线，1为4点在一边，0为分开
long long mind;		//记录四个角点到矩形四个角的min距离，用于一一对应
int flag;
const double coswx = 0.05;   //cos的容忍误差，用于寻找角点,如果是圆角，需要调大
// void duibi(const Mat& pic, Mat& dst, double alpha, double beta)
// {
// 	Mat tmp = pic.clone();
// 	//pic.convertTo()
// }
//
// bool cmp(Vec4i x,Vec4i y)
// {
// 	return
// 	(x[0] - x[2]) * (x[0] - x[2]) + (x[1] - x[3]) * (x[1] - x[3])
// 	>
// 	(y[0] - y[2]) * (y[0] - y[2]) + (y[1] - y[3]) * (y[1] - y[3]);
// }

int main()
{
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);
	img = imread("4.jpg");
	if (img.empty())
	{
		return -1;
	}
	resize(img, img, Size(480, 720));
	bak = img.clone();
	imshow("img", img);
	Mat sz = (Mat_<int>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	filter2D(img, img, -1, sz);
	cvtColor(img, hdt, COLOR_BGR2GRAY, 1);
	GaussianBlur(hdt, hdt2, Size(5, 5), 1.0);
	Canny(hdt2, edge, 60, 120);
	imshow("edge", edge);
	waitKey();
	HoughLinesP(edge, lines, 1, CV_PI / 360, 100, 100, 15);
	const Vec4i* p = nullptr;
	for (i = 0; i < lines.size(); ++i)
	{
		p = &lines[i];
		pt[2 * i].x = (*p)[1];
		pt[2 * i].y = (*p)[0];
		pt[2 * i + 1].x = (*p)[3];
		pt[2 * i + 1].y = (*p)[2];
	}
	lim = 2 * lines.size();
	for (i = 0; i < lim - 1; ++i)
	{
		for (j = i + 1; j < lim; ++j)
		{
			d = (pt[i].x - pt[j].x) * (pt[i].x - pt[j].x) + (pt[i].y - pt[j].y) * (pt[i].y - pt[j].y);
			if (d > maxx)
			{
				maxx = d;
				n1 = pt[i]; f1 = i;
				n2 = pt[j]; f2 = j;
			}
		}
	}
	mincos = 2;
	for (i = 0; i < lim; ++i)
	{
		if (i == f1 || i == f2) continue;
		d1 = (pt[i].x - pt[f1].x) * (pt[i].x - pt[f1].x) + (pt[i].y - pt[f1].y) * (pt[i].y - pt[f1].y);
		d2 = (pt[i].x - pt[f2].x) * (pt[i].x - pt[f2].x) + (pt[i].y - pt[f2].y) * (pt[i].y - pt[f2].y);
		if (d1 < 1000||d2<1000) continue;
		tmpcos = (d1 + maxx - d2) / (2 * sqrt(d1 * maxx));
		if (mincos - tmpcos > coswx)						//允许的直线歪曲程度，越小越严格，范围0->1
		{
			maxd = d1;
			mincos = tmpcos;
			f3 = i;
		}
		else if (abs(mincos - tmpcos) < coswx)
		{
			if (d1 > maxd)
			{
				mincos = tmpcos;
				f3 = i;
				maxd = d1;
			}
		}

	}

	mincos = 2; maxd = 0;	//初始化
	disto3 = (pt[f3].x - pt[f1].x) * (pt[f3].x - pt[f1].x) + (pt[f3].y - pt[f1].y) * (pt[f3].y - pt[f1].y);

	for (i = 0; i < lim; ++i)
	{
		if (i == f1 || i == f3) continue;
		d1 = (pt[i].x - pt[f1].x) * (pt[i].x - pt[f1].x) + (pt[i].y - pt[f1].y) * (pt[i].y - pt[f1].y);
		d3 = (pt[i].x - pt[f3].x) * (pt[i].x - pt[f3].x) + (pt[i].y - pt[f3].y) * (pt[i].y - pt[f3].y);
		if (d1 < 1000||d3<1000) continue;
		tmpcos = (d1 + disto3 - d3) / (2 * sqrt(d1 * disto3));
		if (mincos - tmpcos > coswx)
		{
			maxd = d1;
			mincos = tmpcos;
			f4 = i;
		}
		else if (abs(mincos - tmpcos) < coswx)
		{
			if (d1 > maxd)
			{
				mincos = tmpcos;
				f4 = i;
				maxd = d1;
			}
		}
	}
	d1 = (pt[f1].x - pt[f4].x) * (pt[f1].x - pt[f4].x) + (pt[f1].y - pt[f4].y) * (pt[f1].y - pt[f4].y);
	d2 = (pt[f2].x - pt[f4].x) * (pt[f2].x - pt[f4].x) + (pt[f2].y - pt[f4].y) * (pt[f2].y - pt[f4].y);
	if (d1<sameptdis||d2 < sameptdis)		//计算的第四个点和第二个点或第一个点重合，换另一个角重算
	{
		condition = 1;
		mincos = 2; maxd = 0;
		for (i = 0; i < lim; ++i)
		{
			if (i == f1 || i == f3) continue;
			d1 = (pt[i].x - pt[f1].x) * (pt[i].x - pt[f1].x) + (pt[i].y - pt[f1].y) * (pt[i].y - pt[f1].y);
			d3 = (pt[i].x - pt[f3].x) * (pt[i].x - pt[f3].x) + (pt[i].y - pt[f3].y) * (pt[i].y - pt[f3].y);
			if (d3 < 1000) continue;
			tmpcos2 = (d3 + disto3 - d1) / (2 * sqrt(d3 * disto3));
			if (mincos - tmpcos2 > coswx)						//允许的直线歪曲程度，越小越严格，范围0->1
			{
				maxd = d3;
				mincos = tmpcos2;
				f4 = i;
				condition = 0;
			}
			else if (abs(mincos - tmpcos2) < coswx)
			{
				if (d3 > maxd)
				{
					mincos = tmpcos2;
					f4 = i;
					maxd = d3;
					condition = 0;
				}
			}
		}
	}
	condition = 0;

	//sort(lines.begin(), lines.end(),cmp);

	// d1=(orglines[i][0]-orglines[i][2])*(orglines[i][0]-orglines[i][2])+(orglines[i][1]-orglines[i][3])*(orglines[i][1]-orglines[i][3]);
	// for(iter=orglines.begin();iter!=orglines.end();)
	// {
	// 	//p=&orglines[i];
	// 	d2=((*iter)[0] - (*iter)[2]) * ((*iter)[0] - (*iter)[2]) + ((*iter)[1] - (*iter)[3]) * ((*iter)[1] - (*iter)[3]);
	// 	if(abs(d1-d2)<100)
	// 	{
	// 		iter=orglines.erase(iter);
	// 	}
	// 	else ++iter;
	// 	d1=d2;
	// }
	//
	//
	cout << "n1(" << pt[f1].x << "," << pt[f1].y << ")" << endl;
	cout << "n2(" << pt[f2].x << "," << pt[f2].y << ")" << endl;
	cout << "n3(" << pt[f3].x << "," << pt[f3].y << ")" << endl;
	cout << "n4(" << pt[f4].x << "," << pt[f4].y << ")" << endl;
	cout << "f1=" << f1 << endl;
	cout << "f2=" << f2 << endl;
	cout << "f3=" << f3 << endl;
	cout << "f4=" << f4 << endl;
	cout << "size=" << lines.size() << endl;

	cvtColor(edge, edge, COLOR_GRAY2BGR);
	circle(edge, Point(pt[f1].y, pt[f1].x), 10, Scalar(0, 0, 255), 2);
	imshow("edge",edge);
	waitKey();
	circle(edge, Point(pt[f2].y, pt[f2].x), 10, Scalar(0, 0, 255), 2);
	imshow("edge",edge);
	waitKey();
	circle(edge, Point(pt[f3].y, pt[f3].x), 10, Scalar(0, 0, 255), 2);
	imshow("edge",edge);
	waitKey();
	circle(edge, Point(pt[f4].y, pt[f4].x), 10, Scalar(0, 0, 255), 2);
	imshow("edge",edge);
	waitKey();
	for (i = 0; i < lines.size(); ++i)
	{
		p = &lines[i];
		cout << "l=" << ((*p)[0] - (*p)[2]) * ((*p)[0] - (*p)[2]) + ((*p)[1] - (*p)[3]) * ((*p)[1] - (*p)[3]) << endl;
		line(edge, Point((*p)[0], (*p)[1]), Point((*p)[2], (*p)[3]), Scalar(0, 255, 0), 2);

	}
	imshow("edge", edge);
	waitKey();

	//dst=Mat::zeros(Size(420,594),CV_8UC3);
	Point2f orgp[4], dstp[4];
	orgp[0] = Point(pt[f1].y, pt[f1].x);
	orgp[1] = Point(pt[f2].y, pt[f2].x);
	orgp[2] = Point(pt[f3].y, pt[f3].x);
	orgp[3] = Point(pt[f4].y, pt[f4].x);
	//--------------------对4个角点排序使得与dstp矩形四个角点一一对应
	//换了算法，在图片称近乎45°时，会错误匹配角点，改为x+y
	mind = 99999999999;
	for (i = 0; i <= 3; ++i)
	{
		//d=orgp[i].x*orgp[i].x+orgp[i].y*orgp[i].y;
		d = orgp[i].x + orgp[i].y;
		if (d < mind)
		{
			flag = i;
			mind = d;
		}
	}
	swap(orgp[0], orgp[flag]);
	mind = 99999999999;
	for (i = 1; i <= 3; ++i)
	{
		//d=(orgp[i].x-paperweight+1)*(orgp[i].x-paperweight+1)+orgp[i].y*orgp[i].y;
		d = paperweight - 1 - orgp[i].x + orgp[i].y;
		if (d < mind)
		{
			flag = i;
			mind = d;
		}
	}
	swap(orgp[1], orgp[flag]);
	mind = 99999999999;
	for (i = 2; i <= 3; ++i)
	{
		d = d = paperheight - 1 + orgp[i].x - orgp[i].y;
		if (d < mind)
		{
			flag = i;
			mind = d;
		}
	}
	swap(orgp[2], orgp[flag]);

	//--------------------
	dstp[0] = Point(0, 0);

	dstp[1] = Point(paperweight - 1, 0);
	dstp[2] = Point(0, paperheight - 1);
	dstp[3] = Point(paperweight - 1, paperheight - 1);
	Mat transform = getPerspectiveTransform(orgp, dstp);
	warpPerspective(img, dst, transform, Size(paperweight, paperheight));

	imshow("dst", dst);
	waitKey();
	system("pause");
}
