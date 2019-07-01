#include <iostream>
#include <stdio.h>
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.h>

using namespace cv;
using namespace std;

int main(int argc, char *argv[1])
{

	Mat img = imread("lowContrast.PNG");
	Mat grayImg;
	if (!img.data) {
		cout << "could not load the image" << std::endl;
		waitKey(0);
		return -1;
	}

	
	int histogram[256];
	double probability[256];
	double CumProbability[256];
	int map[256];

	for (int i = 0; i<256; i++) {
		histogram[i] = 0;
		probability[i] = 0.0;
		CumProbability[i] = 0.0;
		map[i] = 0;
	}

	cvtColor(img, grayImg, CV_BGR2GRAY,0);
	Mat Heq = grayImg.clone();

	int h = grayImg.rows;
	int w = grayImg.cols;
	int N = h*w;
	int val = 0;

	cout << "Width" << w << endl;
	cout << "N" << N << endl;

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			val = grayImg.at<uchar>(i, j);
			histogram[val] = histogram[val] + 1;
		}
	}


	for (int i = 0; i < 256; i++) {
		
		probability[i] = (double)histogram[i] / N; //probability of each color level
		
		if (i == 0) {
			CumProbability[i] = probability[i];
		}			
		else
		{
			CumProbability[i] = CumProbability[i-1] + probability[i];
		}
		
		map[i] = cvRound(CumProbability[i]*255); //new gray level map
		
	}
	int index;
	
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			index = 0;
			index = grayImg.at<uchar>(i, j);
			Heq.at<uchar>(i,j) = (uchar)map[index];
		}
	}

	//cout <<"Max :"<< max << endl;
	//Mat originalHistogram(301, 260, CV_8UC1, Scalar(255)); //only the resolution of histogram size
	//int hist[256];
	//double maxd = max;
	//for (int i = 0; i < 256; i++) {
	//	hist[i] = cvRound(double(his[i] / maxd) * 300);
	//	Point pt1 = Point(i, 300 - hist[i]);
	//	Point pt2 = Point(i, 300 );
	//	line(originalHistogram, pt1, pt2, Scalar(0), 1, 8, 0);
	//}
	
	
	
	cvNamedWindow("Original Image");  // create a new window with a name
	imshow("Original Image", grayImg);

	cvNamedWindow("New Image"); 
	imshow("New Image", Heq);

	
	
	
	cvWaitKey(0);   // wait for a hey input and hold
	cvDestroyWindow("Original Image"); //release the memory allocation
	//cvDestroyWindow("Original Histogram");
	
	cvDestroyWindow("New Image");
	
	img.release(); //release the memory allocation
	return 0;

}