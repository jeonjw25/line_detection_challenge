# line_detection_challenge

<br><br>  

# 팀원
  
| 😁 | NAME | HOBBY |
|------|------|--------|
| ![전종원](https://user-images.githubusercontent.com/54730375/123045448-e6795a80-d435-11eb-9f87-e20f79e5a43c.PNG) |   [전종원](https://github.com/jeonjw25/)  |   🕹게임, 🏀농구, 🍰맛있는거먹기   |
| ![유희평]()  |   [유희평]()  |       |

<br><br>  

# 목표 & 규정

1. 좌차선(lpos) & 우차선(rpos) 인식.  
  lposl < lpos < lposr & rposl < rpos < rposr 이 되도록 하기.
  
  ![image](https://user-images.githubusercontent.com/54730375/162557907-d6e7669a-00b6-4516-9694-e82baf0bd885.png)  
  
2. 차선이 가려지거나 끊긴경우 끊긴 차선의 pos가 화면의 맨 끝으로 인식되도록 하기.
3. 차선추출 후 offset 400 위치에서의 lpos와 rpos를 30frame 단위로 csv 파일제출(변환코드 제공)
4. 개발언어 무관(멘토들은 c++ 추천하심)
5. 차선인식 알고리즘 상관x
6. 영상파일 제공(밝기 상이, 차선 끊김존재)

![image](https://user-images.githubusercontent.com/54730375/162558142-2733c5cb-5f85-473b-af10-242933aa3bf0.png)  



<br><br>  

# 일정

1. ~4/15 (금) 코드완성  
2. 4/16 (토) 멘토님들의 코드리뷰  
3. 4/17 (일) 발표(시간은 추후 투표)  

<br><br>  

# to-do  

- 동영상 받아오기  
- 가우시안블러
- 평균밝기유지(함수화)
- HSV색공간변환
- 이진화
- 캐니
- 허프
- 노이즈처리(함수화)
- csv 변환(함수화)
