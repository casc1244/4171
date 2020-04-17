#include<opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace std;
int main()
{
	Mat  dstImage_Erode, dstImage_Dilate, dstImage_Open, dstImage_Close;
	cv::Mat binaryMat;
	cv::Mat srcMat = imread("D:\\360downloads\\12.png", 0);
	cv::Mat labelMat;
	cv::Mat statsMat;
	cv::Mat centrMat;
	cv::Mat resultMat;
	// 对图像进行所有像素用 （255- 像素值）
	Mat invertImage;
	srcMat.copyTo(invertImage);
	cv::threshold(invertImage, binaryMat, 0, 255, THRESH_OTSU);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(7, 7));
	morphologyEx(binaryMat, dstImage_Open, MORPH_OPEN, kernel, Point(-1, -1), 1);//开运算
	int nComp = cv::connectedComponentsWithStats(dstImage_Open,labelMat,statsMat,centrMat,8,CV_32S);
	//输出连通域
	for (int i = 0; i < nComp; i++)
	{
		//各个连通域的统计信息保存在
		cout << "connected Components NO. " << i << endl;
		cout << "pixels = " << statsMat.at<int>(i, 4) << endl;
		cout << "width = " << statsMat.at<int>(i, 2) << endl;
		cout << "height = " << statsMat.at<int>(i, 3) << endl;
		cout << endl;
	}
	resultMat = cv::Mat::zeros(srcMat.size(), CV_8UC3);    //显示用图像
	std::vector<cv::Vec3b> colors(nComp);
	colors[0] = cv::Vec3b(0, 0, 0);//背景黑色
	//绘制bounding box
	for (int i = 1; i < nComp; i++)
	{
		Rect bndbox;
		//左上角坐标
		bndbox.x = statsMat.at<int>(i, 0);
		bndbox.y = statsMat.at<int>(i, 1);
		//宽和长
		bndbox.width = statsMat.at<int>(i, 2);
		bndbox.height = statsMat.at<int>(i, 3);
		//绘制
		rectangle(srcMat, bndbox, CV_RGB(255,0, 0), 1, 8, 0);
	}
	imshow("binaryMat", binaryMat);
	imshow("frame", srcMat);
	moveWindow("frame", 0, 20);
	moveWindow("binaryMat", srcMat.cols, 20);
	moveWindow("results", srcMat.cols * 2, 20);
	waitKey(0);

	return 0;
}
