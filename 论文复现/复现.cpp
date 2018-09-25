#include<iostream>
#include<opencv2/opencv.hpp>

/*
	程序基于论文 基于监控视频的高校教室占用率统计系统，吴华宇
	未做最后帧差法，因为该算法不太适合目前手上项目就放弃了，开源给各位一个参考
*/
/*
	开发环境：VS2017 OpenCV3.4
*/

#define WINDOW_NAME "V值图像"

using namespace std;
using namespace cv;

//int g_nV;
//Mat vImage;
//Mat Image;

string int2str(const int &int_temp)
{
	stringstream stream;
	string string_temp;
	stream << int_temp;
	string_temp = stream.str();  //此处也可以用 stream>>string_temp
	return string_temp;
}

vector<Mat> changeHSV(Mat src)		//颜色转换HSV
{
	Mat img_h, img_s, img_v,img_hsv;
	vector<Mat> hsv_vec;
	cvtColor(src, img_hsv, COLOR_RGB2HSV);
	split(img_hsv,hsv_vec);
	return hsv_vec;
}

//void on_Trackbar(int, void *)
//{
//	vImage = Image * (double)(g_nV/100);
//	imshow(WINDOW_NAME,vImage);
//}


int main()
{
	Mat src = imread("C:\\Users\\王茂林\\Desktop\\test1.jpg");		//读取图片
	vector<Mat> img_vec;
	img_vec = changeHSV(src);
	//imshow("H", img_vec[0]);
	//imshow("S", img_vec[1]);
	imshow("V", img_vec[2]);
	Mat vImage;
	vImage = img_vec[2]*0.25;
	imshow("0.25V", vImage);
	//cvNamedWindow(WINDOW_NAME);
	//Image = img_vec[2];
	//createTrackbar("V值大小",WINDOW_NAME,&g_nV,100,on_Trackbar);	
	//on_Trackbar(g_nV, 0);
	/*二值化操作*/
	Mat thresImg20,thresImg15,thresImg25,thresImg30;		//二值化参数测试
	threshold(vImage, thresImg15, 15, 255, THRESH_BINARY_INV);
	threshold(vImage, thresImg20, 20, 255, THRESH_BINARY_INV);
	threshold(vImage, thresImg25, 25, 255, THRESH_BINARY_INV);
	threshold(vImage, thresImg30, 30, 255, THRESH_BINARY_INV);
	imshow("二值化图像",thresImg25);
	

	/*腐蚀膨胀处理*/
	//腐蚀操作
	Mat erodeImg;
	erode(thresImg25, erodeImg, Mat(2, 2, CV_8U),Point(-1,-1), 1);
	imshow("腐蚀处理", erodeImg);
	Mat element;
	element = getStructuringElement(MORPH_RECT,Size(4,4));
	Mat dilateImg;
	dilate(erodeImg, dilateImg, element);
	imshow("膨胀处理", dilateImg);
	/*Canny边缘检测*/
	Mat edgeImg;
	blur(dilateImg, edgeImg, Size(3, 3));
	Canny(edgeImg, edgeImg, 20, 30);
	imshow("Canny检测", edgeImg);
	vector<Vec3f> circles;
	HoughCircles(edgeImg,circles, HOUGH_GRADIENT, 2,50, 20, 30, 30, 50);
	int countNum=0;
	for (int i = 0; i < circles.size(); i++)
	{
		circle(src,Point(circles[i][0],circles[i][1]),circles[i][2],Scalar(147,214,214));
		countNum++;
	}
	putText(src, int2str(countNum), Point(40, 40), FONT_HERSHEY_TRIPLEX, 1, Scalar(57,197,187), 2);
	imshow("结果",src);
	imwrite("C:\\Users\\王茂林\\Desktop\\结果1.jpg",src);
	waitKey();
	return 0;
}