#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>


using namespace cv;
using namespace std;

int dil_elude() {
	Mat cam_data;

	VideoCapture cap(0);//web cam ����
	//fps 1�� ����
	double fps = cap.set(CAP_PROP_FPS, 1);

	//cout << "fps : " << fps << endl;

	if (!cap.isOpened()) {
		cerr << "error onn fail" << endl;
		return -1;
	}

	while (1)
	{
		cap.read(cam_data);
		if (cam_data.empty()) {
			cerr << "camdata empty" << endl;
			break;
		}

		Mat imgproc = cam_data.clone();//cam data �� improc���� �̹��� ����
		Mat	imgsplit[3];//R,G,B�� ��ä�ξ� �̾Ƴ� ����
		split(imgproc, imgsplit);//(�Է��̹���,���);//B(0),G(1),R(2)�� ���´�

		Mat red_find_res;//��� ������ ����
		threshold(imgsplit[2], red_find_res, 200, 255, THRESH_BINARY);
		//Rä���� ������(200�̻��� ����)�� 255�� �ٲ��ָ鼭 redfindres�� �������ش�

		Mat green_find_res;
		threshold(imgsplit[1], green_find_res, 200, 255, THRESH_BINARY);//�ʷ��� 200���� ���� ��

		Mat blue_find_res;
		threshold(imgsplit[0], blue_find_res, 200, 255, THRESH_BINARY);//�Ķ��� 200���� ���� ��

		Mat last_find_res;//��� ���ܵ� ����
		last_find_res = red_find_res - green_find_res - blue_find_res;

		//erode,dil
		int ele_size = 30;//1�̸� 9�� �ȼ�, 30�̸� �ѹ��� 61���ȼ��� ����
		Mat element = getStructuringElement(MORPH_RECT, Size(ele_size * 2 + 1, ele_size * 2 + 1), Point(1, 1));
		//� ���·� ������Ʈ �Ұ�����(�簢��,������, �߾Ӱ�)

		Mat erode_res, dil_res;//ħ�İ� Ȯ�� ��� �����ϴ� ����

		//erode(red_find_res,erode_res,element);//������ ã�� �κп� ���� ħ�� ����
		dilate(red_find_res, dil_res, element);//������ ã�� �κп� ���� Ȯ�� ����.

		erode(red_find_res, erode_res, element);
		dilate(erode_res, erode_res, element);//������ ã�� �κп� ���� Ȯ�� ����.

		dilate(erode_res, erode_res, element);
		dilate(erode_res, erode_res, element);
		dilate(erode_res, erode_res, element);
		erode(erode_res, erode_res, element);
		erode(erode_res, erode_res, element);//������ ã�� �κп� ���� ħ�� ����
											  //Ȯ�� �� �ٽ� ħ��(���� ������ �Բ� ó��)



		imshow("erd", erode_res);
		imshow("dil", dil_res);


		//hls

		Mat Img_HLS;
		Mat Img_HLS_split[3];
		Mat Img_H_Red_0_6;//0~6��
		Mat Img_H_Red_350_360;//0~6��
		Mat Img_H_Red_res;//0~6��
		cvtColor(imgproc, Img_HLS, COLOR_BGR2HLS);
		imshow("hls", Img_HLS);
		split(Img_HLS, Img_HLS_split);

		threshold(Img_HLS_split[0], Img_H_Red_0_6, 3, 255, THRESH_BINARY_INV);
		threshold(Img_HLS_split[0], Img_H_Red_350_360, 175, 255, THRESH_BINARY);//���غ��� ũ�� b,������ w

		Img_H_Red_res = Img_H_Red_0_6 + Img_H_Red_350_360;



		imshow("h0", Img_H_Red_0_6);
		imshow("h350", Img_H_Red_350_360);
		imshow("h_res", Img_H_Red_res);

		



		imshow("cam", cam_data);
		//imshow("th_r_200",red_find_res);

		//imshow("r",imgsplit[2]);
		//imshow("g",imgsplit[1]);
		//imshow("b",imgsplit[0]);

		//imshow("last_find_res",last_find_res);

		if (waitKey(25) >= 0) {
			break;
		}
	}

	return 0;


}