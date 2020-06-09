#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>


using namespace cv;
using namespace std;

int dil_elude() {
	Mat cam_data;

	VideoCapture cap(0);//web cam 연결
	//fps 1로 설정
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

		Mat imgproc = cam_data.clone();//cam data 에 improc으로 이미지 복사
		Mat	imgsplit[3];//R,G,B를 한채널씩 뽑아낼 공간
		split(imgproc, imgsplit);//(입력이미지,출력);//B(0),G(1),R(2)로 나온다

		Mat red_find_res;//결과 저장할 공간
		threshold(imgsplit[2], red_find_res, 200, 255, THRESH_BINARY);
		//R채널의 빨간색(200이상의 빛값)을 255로 바꿔주면서 redfindres에 저장해준다

		Mat green_find_res;
		threshold(imgsplit[1], green_find_res, 200, 255, THRESH_BINARY);//초록이 200보다 높은 영

		Mat blue_find_res;
		threshold(imgsplit[0], blue_find_res, 200, 255, THRESH_BINARY);//파랑이 200보다 높은 영

		Mat last_find_res;//흰색 제외된 빨강
		last_find_res = red_find_res - green_find_res - blue_find_res;

		//erode,dil
		int ele_size = 30;//1이면 9개 픽셀, 30이면 한번에 61개픽셀을 보겠
		Mat element = getStructuringElement(MORPH_RECT, Size(ele_size * 2 + 1, ele_size * 2 + 1), Point(1, 1));
		//어떤 여태로 엘리먼트 할것인지(사각형,사이즈, 중앙값)

		Mat erode_res, dil_res;//침식과 확대 결과 저장하는 변수

		//erode(red_find_res,erode_res,element);//빨간색 찾은 부분에 대한 침식 연산
		dilate(red_find_res, dil_res, element);//빨간색 찾은 부분에 대한 확대 연산.

		erode(red_find_res, erode_res, element);
		dilate(erode_res, erode_res, element);//빨간색 찾은 부분에 대한 확대 연산.

		dilate(erode_res, erode_res, element);
		dilate(erode_res, erode_res, element);
		dilate(erode_res, erode_res, element);
		erode(erode_res, erode_res, element);
		erode(erode_res, erode_res, element);//빨간색 찾은 부분에 대한 침식 연산
											  //확대 후 다시 침식(내부 잡은도 함께 처리)



		imshow("erd", erode_res);
		imshow("dil", dil_res);


		//hls

		Mat Img_HLS;
		Mat Img_HLS_split[3];
		Mat Img_H_Red_0_6;//0~6도
		Mat Img_H_Red_350_360;//0~6도
		Mat Img_H_Red_res;//0~6도
		cvtColor(imgproc, Img_HLS, COLOR_BGR2HLS);
		imshow("hls", Img_HLS);
		split(Img_HLS, Img_HLS_split);

		threshold(Img_HLS_split[0], Img_H_Red_0_6, 3, 255, THRESH_BINARY_INV);
		threshold(Img_HLS_split[0], Img_H_Red_350_360, 175, 255, THRESH_BINARY);//기준보다 크면 b,낮으면 w

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