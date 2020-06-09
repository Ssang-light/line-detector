#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <vector>
#define left_threshold -30//�������� Ƣ�� ���
#define right_threshold 120//�������� Ƣ�� ���
#define left_trigger -20
#define right_trigger 100
#define camera_normalization 10


//�۾��� �ٽ� ����
using namespace cv;
using namespace std;

vector <int> diff_vector;

int deflectionAvg() {
	int sum = 0;
	int count = 0;

	for (int i = 0; i < diff_vector.size(); i++) {
		if (diff_vector[i] >= right_threshold || diff_vector[i] < left_threshold) {
			//continue;
		}
		sum += diff_vector[i];
		count++;
	}
	return sum / count;
}


int main()
{///

	Mat cam_data;
	VideoCapture video("C:\\Users\\Kyung\\Desktop\\image\\test_drive2.mp4");
	//VideoCapture video(0);
	
	//double fps = video.set(CAP_PROP_FPS, 100);
	if (!video.isOpened()) {
		cerr << "error onn fail" << endl;
		return -1;
	}

	while (1) {

		video.read(cam_data);
		if (cam_data.empty()) {
			cerr << "camdata empty" << endl;
			break;
		}


		//Mat image = imread("C:\\Users\\Kyung\\Desktop\\cv_exercise\\road3.jpg", IMREAD_COLOR);		// ��ϵ� ���� �б�
		//CV_Assert(!image.empty());									// �������� ����ó��


		//Mat imgproc = image.clone();
		Mat imgproc = cam_data.clone();

		Mat lineROI = imgproc(Rect(0, imgproc.rows / 2,
			imgproc.cols, imgproc.rows / 2)).clone();

		int half_point = imgproc.cols / 2;

		Mat lineProc =
			Mat::zeros(lineROI.rows, lineROI.cols,
				CV_8UC3);

		Mat grayImg;
		cvtColor(lineROI, grayImg, COLOR_BGR2GRAY);

		Mat cannyImg;
		Mat thImg;
		///
		Mat temp_left = lineROI.clone();
		Mat temp_right = lineROI.clone();
		cout << "��ü ����" << imgproc.cols << endl;

		threshold(grayImg, thImg, 100, 255, THRESH_BINARY);

		Canny(grayImg, cannyImg, 100, 150, 3);//canny�� ȸ�� ȭ�� �־��ָ� �ܰ��� ����

		vector<Vec2f> lines;

		int threshold = 100;//����(���� �ܰ����� �� ������ ��� ������ ���� �׸���)

		HoughLines(cannyImg, lines, 1,
			CV_PI / 180, threshold);

		float theta_min = 30.0f * CV_PI / 180.0f;
		float theta_max = 60.0f * CV_PI / 180.0f;
		float theta_min2 = 110.0f * CV_PI / 180.0f;
		float theta_max2 = 160.0f * CV_PI / 180.0f;




		Mat cannyColor;
		Mat ROITest = lineROI.clone();
		cvtColor(cannyImg, cannyColor, COLOR_GRAY2BGR);
		ROITest = ROITest - cannyColor;
		imshow("ROI CHECK", ROITest);


		/////
		Mat Roiselected = lineROI.clone();
		///


		Point leftP[2], rightP[2], banishP;
		bool bLFound = false, bRFound = false;
		////////////////////////
		int max = 0;
		int max2 = 0;
		int min = imgproc.cols;
		for (int k = 0; k < lines.size(); k++) {

			float rho2 = lines[k][0], theta2 = lines[k][1];
			int candidate_left = rho2 / sin(theta2);
			int candidate_right = rho2 / sin(theta2);
			//x �����߿��� �ִ밪�� ã�´�.(������)
			if ((theta2 > theta_min && theta2 < theta_max)) {
				if (candidate_left < 700) {
					if (max < candidate_left) {
						max = candidate_left;
					}
				}

			}
			//x �����߿��� �ּҰ��� ã�´�(������)
			if ((theta2 > theta_min2 && theta2 < theta_max2)) {
				/*	cout << "check" << endl;
					if (abs(max2) < abs(candidate_right)) {
						max2 = candidate_right;
					}*/


				if (abs(min) > abs(candidate_right)) {

					//if (candidate_right <-200) {
					if (candidate_right < 0) {

						cout << "min" << min << endl;
						cout << "candi" << candidate_right << endl;
						min = candidate_right;
					}
				}
			}

		}
		cout << "max" << max << "min" << max2 << endl;
		//////////////////////////
		for (int i = 0; i < lines.size(); i++)
		{
			float rho = lines[i][0], theta = lines[i][1];

			Scalar color = Scalar(0, 0, 255);
			bool bSearch = false;


			Point pt11(0, rho / sin(theta));
			Point pt21(ROITest.cols, (rho - ROITest.cols * cos(theta)) / sin(theta));
			line(ROITest, pt11, pt21, color, 1);



			/*Point pt11(0, rho/sin(theta));
			Point pt21(cannyImg.cols,
			(rho - cannyImg.cols*cos(theta)/ sin(theta)));
			line(lineROI, pt11, pt21, color, 1);*/
			//////
			if ((theta > theta_min && theta < theta_max) || (theta > theta_min2 && theta < theta_max2)) {
				line(Roiselected, pt11, pt21, color, 1);
			}
			////
			//�� ��, x ������ �߾ƿ��� ���� ����� ������ �̱�9������


			if (theta > theta_min && theta < theta_max
				&& bLFound == false)
			{
				///

				if (pt11.y == max) {//�������� �߿��� ���� �߾ƿ� ����� ���� �����Ͽ�

					line(temp_left, pt11, pt21, color, 1);

					////

					color = Scalar(255, 0, 0);
					bSearch = true;
					bLFound = true;

					Point pt1(0, rho / sin(theta));
					Point pt2(cannyImg.cols,
						(rho - cannyImg.cols * cos(theta)) / sin(theta));
					line(lineProc, pt1, pt2, color, 10);
					leftP[0] = pt1;
					leftP[1] = pt2;

					Point pt11(0, rho / sin(theta));
					Point pt21(ROITest.cols, (rho - ROITest.cols * cos(theta)) / sin(theta));
					line(ROITest, pt11, pt21, color, 1);

					leftP[0] = Point(pt11);
					leftP[1] = Point(pt21);

				}
			}//
			if (theta > theta_min2 && theta < theta_max2
				&& bRFound == false)
			{
				cout << "pt11.y" << pt11.y << endl;
				//cout << "min" << min << endl;
				if (pt11.y == min) {//�������� �߿��� ���� �߾ƿ� ����� ���� �����Ͽ�


					line(temp_right, pt11, pt21, color, 1);
					color = Scalar(0, 0, 255);
					bSearch = true;
					bRFound = true;

					Point pt1(0, rho / sin(theta));
					Point pt2(cannyImg.cols,
						(rho - cannyImg.cols * cos(theta)) / sin(theta));
					line(lineProc, pt1, pt2, color, 10);

					rightP[0] = pt1;
					rightP[1] = pt2;

					Point pt11(0, rho / sin(theta));
					Point pt21(ROITest.cols, (rho - ROITest.cols * cos(theta)) / sin(theta));
					line(ROITest, pt11, pt21, color, 1);

					rightP[0] = Point(pt11);
					rightP[1] = Point(pt21);
				}
			}
		}

		imshow("ROI CHECK1", ROITest);
		//////////////
	//if(rightP[1].x != 0&&rightP[1].y != 0 && rightP[0].x!=0 && rightP[0].y != 0 && leftP[1].x!=0 && leftP[1].y != 0 && leftP[0].x!=0 && leftP[0].y != 0)
	//if ((rightP[1].x != 0 && rightP[1].y != 0 )||( rightP[0].x != 0 && rightP[0].y != 0) || (leftP[1].x != 0 && leftP[1].y != 0 )|| (leftP[0].x != 0 && leftP[0].y != 0))

		
		float leftLineA = (float)(leftP[1].y - leftP[0].y) / (float)(leftP[1].x - leftP[0].x);
		float leftLineB = leftP[1].y - leftLineA * leftP[1].x;

		float rightLineA = (float)(rightP[1].y - rightP[0].y) / (float)(rightP[1].x - rightP[0].x);
		float rightLineB = rightP[1].y - rightLineA * rightP[1].x;

		banishP.x = (int)((rightLineB - leftLineB) / (leftLineA - rightLineA));
		banishP.y = (int)(leftLineA * banishP.x + leftLineB);
		if(banishP.x>0&& banishP.y>-10)
		{
		cout << banishP.x << "banishp" << endl;
		cout <<banishP.y<<"abnishpy"<<endl;
		Scalar color = Scalar(0, 255, 0);

		lineROI = lineROI + lineProc;

		imshow("ROI CHECK2", ROITest);

		cout << banishP.x << "," << banishP.y << endl;
		int Lline, Rline = 0;
		for (int i = banishP.x * 3; i > 0; i--)
		{
			uchar* ptr_mat = lineProc.ptr(lineProc.rows - 50);
			if (ptr_mat[i] > 0)
			{
				cout << "i" << i << endl;
				ptr_mat[i] = 255;
				cout << "find l:" << i / 3 << endl;
				cout << "diff l:" << banishP.x - (i / 3) << endl;
				cout << "banish.x" << banishP.x << endl;
				Lline = banishP.x - (i / 3);
				cv::line(lineProc, Point(i / 3, 0), Point(i / 3, lineProc.rows), Scalar(255, 0, 0), 1);

				break;
			}
		}

		for (int i = banishP.x * 3; i < lineProc.cols * 3; i++)
		{
			uchar* ptr_mat = lineProc.ptr(lineProc.rows - 50);
			if (ptr_mat[i] > 0)
			{
				ptr_mat[i] = 255;
				cout << "find r:" << i / 3 << endl;
				cout << "diff r" << (i / 3) - banishP.x << endl;
				cout << "banish.x" << banishP.x << endl;
				Rline = (i / 3) - banishP.x;

				cv::line(lineProc, Point(i / 3, 0), Point(i / 3, lineProc.rows), Scalar(255, 0, 0), 1);
				break;
			}
		}

		line(lineProc, banishP, Point(banishP.x, cannyImg.rows), color, 4);
		line(ROITest, banishP, banishP, color, 4);

		cout << "diff (- L, + R)" << Rline - Lline << endl;
		if (Rline - Lline >= 150 || Rline - Lline <= -40) {
			cout << "               " << "trigger accure" << endl;
		}



		diff_vector.push_back(Rline - Lline);

	}
	//diff_vector = 

	



	imshow("gray", grayImg);
	imshow("canny", cannyImg);
	imshow("th", thImg);
	imshow("line", lineProc);
	imshow("ROI", lineROI);
	imshow("ROIselected", Roiselected);
	imshow("temp_left", temp_left);
	imshow("temp_right", temp_right);
	//
	
	

	if (waitKey(25) >= 0) {
		break;
		}
	
	}
	//file write
	ofstream writeFile("C:\\Users\\Kyung\\Desktop\\image\\diff_out.txt");

	if (writeFile.is_open()) {

		for (int i = 0; i < diff_vector.size(); i++) {
			if (diff_vector[i] >= right_threshold || diff_vector[i] < left_threshold) {
				continue;
			}
			if (diff_vector[i] >= right_trigger || diff_vector[i] < left_trigger) {
				writeFile << "trigger_activate" << " " << endl;
			}
			writeFile << diff_vector[i] << endl;
		}
		writeFile << "avg =" << deflectionAvg() << endl;
	}
	writeFile.close();
	if (deflectionAvg > 0) {
		cout << "deflected on left side" << endl;
	}
	if (deflectionAvg <=0) {
		cout << "deflected on right side" << endl;
	}
	cout << "avg =" << deflectionAvg() << endl;

	
	
	return 0;

}






/*
��ü ���� �ð�
��� �ӵ�
����(���� ���Ƿ�)
�ް��� Ƚ��(���� ���Ƿ�)
�ް��� Ƚ��(���� ���Ƿ�)





*/


//�������� 74��ŭ ġ������ �ִ�
//���� 74�ȼ��� ���� �� �������� ����Ͽ� ����ϸ� �ȴ�.

//�¿� ���� ���� �κ� trigger
//�¿� ���� ������� �� �� ���ε���
//���� �ٲٱ� �˰��� ����