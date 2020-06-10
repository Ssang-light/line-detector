# line-detector

## material = detect line + caculate tiltedness
## 내부기능 = 차선검출 + 좌우 편향치 계산

1. <ROI (region of interest) 설정>


촬영된 운전 영상의 윗부분은 필요가 없다(관심영역이 차선 부분이기 때문에 , 보편적으로 row 기준 1/2 지점부터 row의 끝까지만 사용한다


보편적으로 영상에서 좌표계는 좌측 상단이 0,0이고 vertical 축이 x 축이고 horizontal 축이 y축으로 사용한다.

----------------------------------
2. <canny edge 알고리즘 적용>

ROI 에서 차선을 추출하기 위해 사용할 알고리즘에는 canny edge 를 사용하는데, canny edge를 사용하기 위해서는 컬러 상태를 GRAYSCALE 로 만들어준다 

Cv2.GRAYSCALE은 한 픽셀당 256 레벨의 밝기 차이를 표현 할 수 있는 회색조의 이미지 체계이다. 

2-1. Grayscale로 바뀐 이미지에 canny edge를 적용한다.


canny edge는 기본적으로 gradient 값을 사용하는 알고리즘이다. edge라는 것은 결국 grayscale에서의 밝기의 급격한 변화이기 때문에, 이러한 변화 폭을 미분값으로 계산할 수 있다. 이러한 기울기값이 크다는 것은 색의 변화가 상당히 강하다는 것으로 해석 할 수 있으며, 해당픽셀은 곧 edge를 구성하는 픽셀이라는 판단이 가능하다.

------------------------------------
3. <Hough 변환 적용하기 >


hough 변환은 주로 물체 인식이나 선분 탐색에 많이 사용되는 알고리즘이다. 간단하게 알고리즘을 설명하면 다음과 같다

3-1
카타시안 좌표계를 사용할 경우 보통 y=mx+c 형태로 나타나겠지만 극좌표계를 사용하면 r 과 세타(각)으로 표현할 수 있다.
3-2
픽셀별로 세타 값을 1도씩 늘려가며 0 - 180도로 r 만큼의 파워로 선분을 그린다.(극단적으로 표현하면 각 정점별로 "*" 모양으로 그려질 것이다.
3-3
정점들에서 그려진 별 모양의 선분들이 상호간 겹쳐지는 영역이 나올 것이고 , 일정비율(threshold) 이상의 선분들이 겹칠 경우에는 이것이 완벽한 edge라는 결론을 낼 수 있다.
3-4 
검출된 선분들 중에서 일정 각도(차선을 이루는 보편적인 각도)를 설정하여 이외의 선분들은 화면에서 제외시킨다.(좌측 차선 = 40 - 60도)(우측차선 = 110 - 140도)

------------------------------------

4. <선분 추출>

완성된 선들 중에서 그림의 중심으로부터 가장 가까운 선분 두개("/\\")를 추출해야 한다
가장 가까운 것이라는 기준을 잡은 이유는 운행의 안전상 가장 안쪽의 선이 차선의 가장 내부 선분이기 때문에 보수적인 방법으로 적합하다고 생각했기 때문이다.
4-2
"/\\" 두 선분의 만나는 점을 기준으로 이미지의 하단과 수직한 선분을 하나 긋는다"/|\\"
이 중심점이 일정 기준 점으로부터 얼마나 좌측, 우측으로 치우쳤는지를 계산하여 좌우 편향 정도를 검사한다
4-3
카메라의 설치 방법과 초기 각도에 따라 이미지에서 이루는 각도가 실제 중앙 운행 정도와 다르게 나타날 수 있기 때문에 이를 고려하여 어느정도 조정이 필요하다.(prcatical 하게 조정해야 함)

------------------------------------

5. <예외처리>
선분중에서 곡선주행이 이루어지거나 선분을 찾을 수 없는 경우에는 해당 선분을 detect를 시도하지 않고 찾을 수 있는 선분(좌 혹은 우)만 찾아서 그냥 표시해준다.(좌우 편향 계산 안함)

