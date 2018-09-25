#include<iostream>
#include<opencv2/opencv.hpp>

/*
	����������� ���ڼ����Ƶ�ĸ�У����ռ����ͳ��ϵͳ���⻪��
	δ�����֡�����Ϊ���㷨��̫�ʺ�Ŀǰ������Ŀ�ͷ����ˣ���Դ����λһ���ο�
*/
/*
	����������VS2017 OpenCV3.4
*/

#define WINDOW_NAME "Vֵͼ��"

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
	string_temp = stream.str();  //�˴�Ҳ������ stream>>string_temp
	return string_temp;
}

vector<Mat> changeHSV(Mat src)		//��ɫת��HSV
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
	Mat src = imread("C:\\Users\\��ï��\\Desktop\\test1.jpg");		//��ȡͼƬ
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
	//createTrackbar("Vֵ��С",WINDOW_NAME,&g_nV,100,on_Trackbar);	
	//on_Trackbar(g_nV, 0);
	/*��ֵ������*/
	Mat thresImg20,thresImg15,thresImg25,thresImg30;		//��ֵ����������
	threshold(vImage, thresImg15, 15, 255, THRESH_BINARY_INV);
	threshold(vImage, thresImg20, 20, 255, THRESH_BINARY_INV);
	threshold(vImage, thresImg25, 25, 255, THRESH_BINARY_INV);
	threshold(vImage, thresImg30, 30, 255, THRESH_BINARY_INV);
	imshow("��ֵ��ͼ��",thresImg25);
	

	/*��ʴ���ʹ���*/
	//��ʴ����
	Mat erodeImg;
	erode(thresImg25, erodeImg, Mat(2, 2, CV_8U),Point(-1,-1), 1);
	imshow("��ʴ����", erodeImg);
	Mat element;
	element = getStructuringElement(MORPH_RECT,Size(4,4));
	Mat dilateImg;
	dilate(erodeImg, dilateImg, element);
	imshow("���ʹ���", dilateImg);
	/*Canny��Ե���*/
	Mat edgeImg;
	blur(dilateImg, edgeImg, Size(3, 3));
	Canny(edgeImg, edgeImg, 20, 30);
	imshow("Canny���", edgeImg);
	vector<Vec3f> circles;
	HoughCircles(edgeImg,circles, HOUGH_GRADIENT, 2,50, 20, 30, 30, 50);
	int countNum=0;
	for (int i = 0; i < circles.size(); i++)
	{
		circle(src,Point(circles[i][0],circles[i][1]),circles[i][2],Scalar(147,214,214));
		countNum++;
	}
	putText(src, int2str(countNum), Point(40, 40), FONT_HERSHEY_TRIPLEX, 1, Scalar(57,197,187), 2);
	imshow("���",src);
	imwrite("C:\\Users\\��ï��\\Desktop\\���1.jpg",src);
	waitKey();
	return 0;
}