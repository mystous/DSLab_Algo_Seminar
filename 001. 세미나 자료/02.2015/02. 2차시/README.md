DSLab_Algo_Seminar
==================
고려대학교 분산슈퍼컴퓨팅랩 C/C++ 세미나

2/17 2차 세미나
 
    주제
     C/C++ Syntax overview(다시)
        - 김혜민: 변수, 입출력, 제어문, 관계 연산자, 논리 연산자, 비트 연산자, 쉬프트 연산자, 연산 규칙(연산 순위, 결합 순서, 산술 변환)
        - 김도형: 클래스(상속, 인터페이스), 연산자 오버로딩, 가상함수(Virtual, pure virtual), 템플릿, try-catch, 네임스페이스
        - 서재민: 함수 호출 방식(값 호출, 참조 호출, 출력용 인수, C++의 참조 호출), 전처리기, 전역 변수와 지역 변수, 정적 변수(정적 변수, 레지스터 변수, 정적 함수)
        - 황두현: 배열, 포인터, 이중 포인터, 문자열, 구조체, 공용체, 

     
     Matrix Vector Multiplyer
        - 소스코드 이름: 20150216_[본인 영문명, 별명이나 닉네임 안됨]_Baseball.c[or cpp]
        - 조건 사항
                matrix.dat와 vector_input.dat는 형식이나 파일 명이 올바르지 않을 수 있음
                matrix의 데이터 중 저정되어 있지 않는 element 존재 가능(이때는 계산 안함)
                m과 n의 크기는 제한이 없음
                Row, Column, Real, Imaginary 모두 double 형
                Code는 모두 ansi c 규격에 맞게 작성
                Row index, Column index는 Zero-based
                Program logic을 PPT 파일로 별도로 제작
        - 판단 방법
                파일 읽기 실패(실패시 마다, -5)
                결과 비교(다른 사람과 비교시 다른 결과, -5)
                대용량 파일 읽기 시간(순위 별로 2등부터 추가 감점, -5, -10, -15)
                계산 시간(순위 별로 2등부터 추가 감점, -5, -10, -15)
                결과 파일 저정 형식(오류로 저장시 -5)
                Segment fault시(-10)
                무한 루프(발생시 마다, -5)
        - 시간 측정 함수 Reference/TimeMeasurement.h, .cpp를 그대로 사용하거나 참고하여 작성
                시간은 측정은 아래 두가지 출력
                   > Matrix, Vector 파일 읽기 시작해서 다 읽었을때까지(예외 처리 포함) 측정하여 화면에 출력
                   > Matrix Vector multiplier 시작과 종료 측정하여 화면에 출력
                   > ex     
                           start_time_measurement(); 
                           // Do something
                           end_time_measurement(); // 시각이 자동 출력됨
                

