DSLab_Algo_Seminar
==================
고려대학교 분산슈퍼컴퓨팅랩 C/C++ 세미나

3/6 3차 세미나
 
    주제
     C/C++ Syntax overview
        - 황두현: 배열, 포인터, 이중 포인터, 문자열, 구조체, 공용체, 

     
     Matrix Vector Multiplyer
        - 소스코드 이름: 20150216_[본인 영문명, 별명이나 닉네임 안됨]_Baseball.c[or cpp]
        - 시간 측정 결과 파일 작성(형식 자유)
        - 조건 사항
                지금 만들어진 소스를 병렬화 할것(MPI, Multi Thread, GPGPU)
                데이터 파일은 MVMul_data_generator.exe로 20,000 * 2,000 으로 만들어서 사용
        - 판단 방법
                병렬 처리 이전 소스와 시간 비교(몇배 빨라 졌나)
                병렬수: 2, 4, 8, 16, 32, 64
