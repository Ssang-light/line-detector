//#include <opencv2/opencv.hpp>
//#include <opencv2/imgproc.hpp>
//using namespace cv;
//using namespace std;
//
//int main()
//{
//	Mat image = imread("C:\\Users\\Kyung\\Desktop\\cv_exercise\\high_way1.jpg", IMREAD_COLOR);		// 명암도 영상 읽기
//	CV_Assert(!image.empty());									// 영상파일 예외처리
//
//
//	Mat imgproc = image.clone();
//	Mat lineROI = imgproc(Rect(0, imgproc.rows / 2,imgproc.cols, imgproc.rows / 2)).clone();
//	//ROI = lower half of the original image
//
//	Mat lineProc =
//		Mat::zeros(lineROI.rows, lineROI.cols,
//			CV_8UC3);//ROI의 가로 세로만큼 3채널 색으로 초기화 (zeros)//image 하단부만 자른 이미지에 대해
//
//	Mat grayImg;
//	cvtColor(lineROI, grayImg, COLOR_BGR2GRAY);//graycolor convert
//
//	Mat cannyImg;
//	Mat thImg;
//
//	threshold(grayImg, thImg, 100, 255, THRESH_BINARY);
//
//	Canny(grayImg, cannyImg, 100, 150, 3);
//
//	vector<Vec2f> lines;
//
//	int threshold = 80;
//
//	HoughLines(cannyImg, lines, 1,
//		CV_PI / 180, threshold);//180도에 해당하는 직선 다 찾아보고 많은 직선들 지나는 직선들 라인 생성
//
//	float theta_min = 40.0f * CV_PI / 180.0f;
//	float theta_max = 60.0f * CV_PI / 180.0f;//좌측선 40~60
//	float theta_min2 = 110.0f * CV_PI / 180.0f;
//	float theta_max2 = 140.0f * CV_PI / 180.0f;//우측선 110~140
//
//
//	Mat cannyColor;
//	Mat ROITest = lineROI.clone();
//	cvtColor(cannyImg, cannyColor, COLOR_GRAY2BGR);
//	ROITest = ROITest - cannyColor;
//	imshow("ROI CHECK", ROITest);
//
//
//
//
//
//
//	Point leftP[2], rightP[2], banishP;//좌츠선(점 두개), 우측선(점 두개), 소실점
//	bool bLFound = false, bRFound = false;
//
//	for (int i = 0; i < lines.size(); i++)
//	{
//		float rho = lines[i][0], theta = lines[i][1];
//
//		Scalar color = Scalar(0, 0, 255);
//		bool bSearch = false;
//
//
//		Point pt11(0, rho / sin(theta));
//		Point pt21(ROITest.cols, (rho - ROITest.cols * cos(theta)) / sin(theta));
//		line(ROITest, pt11, pt21, color, 1);
//
//		//Point pt11(0, rho/sin(theta));
//		//Point pt21(cannyImg.cols,
//		//(rho - cannyImg.cols*cos(theta)/ sin(theta)));			
//		//line(lineROI, pt11, pt21, color, 1);
//
//
//		if (theta > theta_min && theta < theta_max
//			&& bLFound == false)
//		{
//			color = Scalar(255, 0, 0);
//			bSearch = true;
//			bLFound = true;
//
//			Point pt1(0, rho / sin(theta));
//			//Point pt2(cannyImg.cols,(rho - cannyImg.cols*cos(theta)/ sin(theta)));			
//			Point pt2(cannyImg.cols, (rho - cannyImg.cols * cos(theta)) / sin(theta));
//			line(lineProc, pt1, pt2, color, 4);
//			leftP[0] = pt1;
//			leftP[1] = pt2;
//		}    
//		if (theta > theta_min2 && theta < theta_max2
//			&& bRFound == false)
//		{
//			color = Scalar(0, 0, 255);
//			bSearch = true;
//			bRFound = true;
//
//			Point pt1(0, rho / sin(theta));
//			//Point pt2(cannyImg.cols,(rho - cannyImg.cols*cos(theta)/ sin(theta)));			
//
//			Point pt2(cannyImg.cols, (rho - cannyImg.cols * cos(theta)) / sin(theta));
//			line(lineProc, pt1, pt2, color, 4);
//
//			rightP[0] = pt1;
//			rightP[1] = pt2;
//		}
//	}
//
//	imshow("ROI CHECK1", ROITest);
//
//	float leftLineA = (float)(leftP[1].y - leftP[0].y) / (float)(leftP[1].x - leftP[0].x);//angle of left line
//	float leftLineB = leftP[1].y - leftLineA * leftP[1].x;
//
//	float rightLineA = (float)(rightP[1].y - rightP[0].y) / (float)(rightP[1].x - rightP[0].x);//angle of right line
//	float rightLineB = rightP[1].y - rightLineA * rightP[1].x;
//
//	banishP.x = (int)((rightLineB - leftLineB) / (leftLineA - rightLineA));//angle
//	banishP.y = (int)(leftLineA * banishP.x + leftLineB);
//
//
//	Scalar color = Scalar(0, 255, 0);
//
//	
//	int Lline, Rline;
//	for (int i = banishP.x * 3; i > 0; i--)
//	{
//		uchar* ptr_mat = lineProc.ptr(lineProc.rows - 50);
//		if (ptr_mat[i] > 0)
//		{
//			ptr_mat[i] = 255;
//			cout << "find l:" << i / 3 << endl;
//			cout << "diff l:" << banishP.x - (i / 3) << endl;
//			Lline = banishP.x - (i / 3);
//			cv::line(lineProc, Point(i / 3, 0), Point(i / 3, lineProc.rows), Scalar(255, 0, 0), 1);
//
//			break;
//		}
//	}
//
//	for (int i = banishP.x * 3; i < lineProc.cols * 3; i++)
//	{
//		uchar* ptr_mat = lineProc.ptr(lineProc.rows - 50);
//		if (ptr_mat[i] > 0)
//		{
//			ptr_mat[i] = 255;
//			cout << "find r:" << i / 3 << endl;
//			cout << "diff r" << (i / 3) - banishP.x << endl;
//			Rline = (i / 3) - banishP.x;
//			cv::line(lineProc, Point(i / 3, 0), Point(i / 3, lineProc.rows), Scalar(255, 0, 0), 1);
//			break;
//		}
//	}
//
//
//	line(lineProc, banishP, Point(banishP.x, cannyImg.rows), color, 4);
//	lineROI = lineROI + lineProc;
//
//	imshow("gray", grayImg);
//	imshow("canny", cannyImg);
//	imshow("th", thImg);
//	imshow("line", lineProc);
//	imshow("ROI", lineROI);
//
//
//
//
//	waitKey();
//	return 0;
//} 