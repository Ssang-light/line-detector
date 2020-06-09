//#include <opencv2/opencv.hpp>
//#include <opencv2/imgproc.hpp>
//
//
//using namespace cv;
//using namespace std;
//빨강 추척하는 코드
//
//
//void calc_histo(Mat image, Mat& hist, int bins, int range_max = 256)
//{
//	hist = Mat(bins, 1, CV_32F, Scalar(0));
//	float gap = range_max / (float)bins;
//
//	for (int i = 0; i < image.rows; i++) {
//		for (int j = 0; j < image.cols; j++)
//		{
//			int idx = int(image.at<uchar>(i, j) / gap);
//			hist.at<float>(idx)++;
//		}
//	}
//}
//
//
//void  calc_Histo(const Mat& image, Mat& hist, int bins, int range_max = 256)
//{
//	int		histSize[] = { bins };					// 히스토그램 계급개수
//	float   range[] = { 0, (float)range_max };		// 채널 화소값 범위
//	int		channels[] = { 0 };						// 채널 목록
//	const float* ranges[] = { range };
//
//	calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges);
//}
//
//void draw_histo(Mat hist, Mat& hist_img, Size size = Size(256, 200))
//{
//	hist_img = Mat(size, CV_8U, Scalar(255));
//	float  bin = (float)hist_img.cols / hist.rows;
//	normalize(hist, hist, 0, size.height, NORM_MINMAX);
//
//	for (int i = 0; i < hist.rows; i++)
//	{
//		float  start_x = (i * bin);
//		float  end_x = (i + 1) * bin;
//		Point2f pt1(start_x, 0);
//		Point2f pt2(end_x, hist.at <float>(i));
//
//		if (pt2.y > 0)
//			rectangle(hist_img, pt1, pt2, Scalar(0), -1);
//	}
//	flip(hist_img, hist_img, 0);
//}
//
//
//
//int test() {
//
//	Mat cam_data;
//
//	//VideoCapture cap("C:\\Users\\Kyung\\Desktop\\image\\test1.mp4");//web cam 연결
//
//	VideoCapture cap(0);
//																	//fps 1로 설정
//	//double fps = cap.set(CAP_PROP_FPS, 100);
//
//	//cout << "fps : " << fps << endl;
//
//	if (!cap.isOpened()) {
//		cerr << "error onn fail" << endl;
//		return -1;
//	}
//
//
//	while (1)
//	{
//
//
//		cap.read(cam_data);
//		if (cam_data.empty()) {
//			cerr << "camdata empty" << endl;
//			break;
//		}
//
//
//		///
//		//Mat image = imread("images/histo.jpg", IMREAD_GRAYSCALE);
//		//CV_Assert(!image.empty());
//
//		Mat hist, hist_img;
//		calc_Histo(cam_data, hist, 256);
//		draw_histo(hist, hist_img);
//
//		imshow("hist_img", hist_img);
//		imshow("image", cam_data);
//
//
//
//		///
//
//		Mat imgproc = cam_data.clone();//cam data 에 improc으로 이미지 복사
//		Mat	imgsplit[3];//R,G,B를 한채널씩 뽑아낼 공간
//		split(imgproc, imgsplit);//(입력이미지,출력);//B(0),G(1),R(2)로 나온다
//
//		Mat red_find_res;//결과 저장할 공간
//		threshold(imgsplit[2], red_find_res, 220, 255, THRESH_BINARY);
//		//R채널의 빨간색(200이상의 빛값)을 255로 바꿔주면서 redfindres에 저장해준다
//
//		Mat green_find_res;
//		threshold(imgsplit[1], green_find_res, 200, 255, THRESH_BINARY);//초록이 200보다 높은 영
//
//		Mat blue_find_res;
//		threshold(imgsplit[0], blue_find_res, 200, 255, THRESH_BINARY);//파랑이 200보다 높은 영
//
//		Mat last_find_res;//흰색 제외된 빨강
//		last_find_res = red_find_res - green_find_res-blue_find_res;
//		//////
//
//		int ele_size = 15;
//		Mat element = getStructuringElement(MORPH_RECT, Size(ele_size * 2 + 1, ele_size * 2 + 1), Point(1, 1));
//	
//		Mat erode_res, dil_res;//침식과 확대 결과 저장하는 변수
//
//		//erode(red_find_res,erode_res,element);//빨간색 찾은 부분에 대한 침식 연산
//		dilate(last_find_res, dil_res, element);//빨간색 찾은 부분에 대한 확대 연산.
//
//		erode(last_find_res, erode_res, element);
//		dilate(erode_res, erode_res, element);//빨간색 찾은 부분에 대한 확대 연산.
//
//		dilate(erode_res, erode_res, element);
//		dilate(erode_res, erode_res, element);
//		dilate(erode_res, erode_res, element);
//		erode(erode_res, erode_res, element);
//		erode(erode_res, erode_res, element);//빨간색 찾은 부분에 대한 침식 연산
//				
//											 
//		imshow("erd", erode_res);
//		imshow("dil", dil_res);//확대 후 다시 침식(내부 잡은도 함께 처리)
//
//
//
//		//Mat result = Mat::zeros(erode_res.rows, erode_res.cols, CV_8UC3);//0값으로 초기화 해줌 경계를 그릴 채널 3개 메트릭스
//		
//		//contour//사각형 그리는 부분
//		//Mat result = Mat::zeros(last_find_res.rows,last_find_res.cols,CV_8UC3);//0값으로 초기화 해줌 경계를 그릴 채널 3개 메트릭스
//		Mat result = imgproc.clone();
//		vector<vector<Point> > contours;
//		vector<Vec4i> hierarchy;
//		findContours(dil_res, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
//		for (int i = 0; i < contours.size(); i++)
//		{
//			Scalar line(0, 0, 255);
//			//drawContours(result, contours, i, line);
//			RotatedRect rrect = minAreaRect(contours[i]);
//			Rect boundingrect = rrect.boundingRect();
//
//			if (boundingrect.width * 0.4
//				<= boundingrect.height
//				&& (boundingrect.width * 0.8
//					>= boundingrect.height)) {
//				rectangle(result, boundingrect, line);
//			}
//		}
//		if (!result.empty())
//		{
//			imshow("result", result);
//		}
//
//		imshow("cam", cam_data);
//		//imshow("th_r_200", red_find_res);
//
//		//imshow("r", imgsplit[2]);
//		//imshow("g", imgsplit[1]);
//		//imshow("b", imgsplit[0]);
//
//		//imshow("last_find_res", last_find_res);
//
//		if (waitKey(25) >= 0) {
//			break;
//		}
//	}
//
//	return 0;
//
//	//4th alver
//	//threshold uper bounded by the lower bound prescriped;
//}