#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <direct.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
using namespace std;
using namespace cv;
int pic_id=1;
char path[1000];
char save_path[1000];
int i, j;
int interval = 30;
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

void on_Trackbar(int, void*)
{
	if (interval <= 0) interval = 1;
}

int main()
{
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);
	cout << "输入文件路径：";
	cin >> path;
	CorrectPath(path);
	Mat img;
	VideoCapture cap = VideoCapture(path);
	cout << "cap.isOpened() = " << cap.isOpened() << endl;
	if (!cap.isOpened())
	{
		cout << "Open video error."<<endl;
		system("pause");
		return -1;
	}
	system("pause");

	//create pic

	//
	namedWindow("video", WINDOW_FREERATIO);
	createTrackbar("每帧间隔ms", "video", &interval, 1000, on_Trackbar);
	cap >> img;
	while (!img.empty()) {
		sprintf(save_path,"pic/%08d.jpg",pic_id);
		++pic_id;
		imwrite(save_path,img);
		imshow("video", img);
		if (waitKey(interval) != -1) break;
		cap >> img;
		
	}
	destroyAllWindows();
	cout << "The end.\n";
	system("pause");
	
}
