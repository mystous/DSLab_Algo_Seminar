// matrix.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include "TimeMeasurement.h"
#include <process.h>
#include <Windows.h>

using namespace std;

#define MAX_SIZE 1024
#define THREAD_N 64


class Complex{
private:
	double real;
	double image;
public :
	Complex() {}
	Complex(double r, double i)
	{
		real=r;
		image=i;
	}
	Complex operator +(Complex C);
	Complex operator -(Complex C);
	Complex operator *(Complex C);

	double getReal()
	{
		return real;
	}
	double getImage()
	{
		return image;
	}

	void setReal(double _real)
	{
		real = _real;
	}
	void setImage(double _image)
	{
		image = _image;
	}

	void OutComplex()
	{
		printf("%lf + %lf i\n", real, image);
	}
};

Complex Complex::operator +(Complex C)
{
	Complex T;
	T.real = real+C.real;
	T.image = image + C.image;
	return T;
}

Complex Complex::operator -(Complex C)
{
	Complex T;
	T.real = real-C.real;
	T.image = image-C.image;
	return T;
}

Complex Complex::operator *(Complex C)
{
	Complex T;
	T.real = (real*C.real) - (image*C.image);
	T.image = (image*C.real) + (real*C.image);
	return T;
}

unsigned int WINAPI ThreadFunction(void *arg);

struct _buf{
	double mnBuffer[MAX_SIZE];
	double n1Buffer[MAX_SIZE/2];
};
struct _buf buf; 

Complex	vecEle(0,0);
HANDLE hMutex;

int _tmain(int argc, _TCHAR* argv[])
{

	double matRow=0, matCol=0;
	FILE *mnMatrix;
	FILE *n1Matrix;
	FILE *outText;
	FILE *vecOut;
	fopen_s(&mnMatrix,"matrix.dat","rb");
	fopen_s(&n1Matrix,"vector_input.dat","rb");
	fopen_s(&outText,"out.txt","w");
	fopen_s(&vecOut,"vector_output.dat","wb");
	start_time_measurement();

	/****************매트릭스 크기**********************/
	long int point;
	point = 4*sizeof(double);
	point = -point;
	if(_fseeki64(mnMatrix, point, SEEK_END)==-1)
	{
		printf("오류\n");
	}


	fread(buf.mnBuffer,sizeof(double),4,mnMatrix);
	matRow=buf.mnBuffer[0];
	matCol=buf.mnBuffer[1];
	int matSize = (int)(matRow+1) * (int)(matCol+1);
	printf("매트릭스 사이즈 row %f col %f  row * col = %d\n", matRow, matCol, matSize);
	int readSize=0;
	int size=0;
	if(_fseeki64(mnMatrix, 0, SEEK_SET)==-1)
	{
		printf("오류\n");
	}
	while(size=fread(buf.mnBuffer,sizeof(double),MAX_SIZE,mnMatrix))
	{
		readSize+=size / 4 ;
	}
	if(readSize == matSize){
		printf("매트릭스 크기 %d 빠진게 없습니다. \n", readSize); 
	}else{
		printf("매트릭스 요소가 빠졌습니다.\n");
		exit(1);
	}


	/******************* 행 렬 검 사 **************************/
	if(_fseeki64(mnMatrix, 0, SEEK_SET)==-1)
	{
		printf("오류\n");
	}
	if(_fseeki64(n1Matrix, 0, SEEK_SET)==-1)
	{
		printf("seek 오류\n");
	}
	
	//test    //vector 개수 
	volatile int test=0;
	int result=0;
	while(result=fread(buf.n1Buffer,sizeof(double),MAX_SIZE/2,n1Matrix))
	{
		test+=result;
	}
	test = test/2;
	int _matCol = (int)matCol+1;
	printf("matCol %d vector %d\n", _matCol, test);
	if(_matCol != test){
		printf("에러 열 개수 맞지 않음\n");
		return 1;
	}
	
	

	/******************* 연 산 **************************/
	if(_fseeki64(mnMatrix, 0, SEEK_SET)==-1)
	{
		printf("seek 오류\n");
	}
	if(_fseeki64(n1Matrix, 0, SEEK_SET)==-1)
	{
		printf("seek 오류\n");
	}
	

	///////////////////////////////////////////////////////////
	int readNum0=0;
	int readNum1=0;
	if( ((matCol+1)*4)<MAX_SIZE )
	{
		readNum0=((int)(matCol+1)*4);
		readNum1=readNum0/2;
	}else{
		readNum0=MAX_SIZE;
		readNum1=readNum0/2;
	}
	
	//int index0=0, index1=0;
	int t=0;
	
	hMutex = CreateMutex(NULL, FALSE, NULL);
	for(int i=0 ; i<(matRow+1) ; i++) // 계산하는 횟수(행수 만큼 연산)
	{
		
		int j=0;
		int cnt=0;	// 읽은 원소 갯수
		int readCount1=fread(buf.n1Buffer,sizeof(double), MAX_SIZE/2, n1Matrix); //복소수이기 때문에 실수 허수 2개씩
		int readCount0=fread(buf.mnBuffer,sizeof(double), (readCount1*2), mnMatrix); // 행 열 추가 따라서 x2
		int colCnt = readCount1/2;
		while(1)
		{
			int threadCnt=0;
			HANDLE threadList[THREAD_N];
			for(cnt ; cnt < (readCount1/2) && threadCnt <THREAD_N ; threadCnt++,cnt++) //열수나 쓰레드 수만큼  
			{
				threadList[threadCnt]=(HANDLE)_beginthreadex(NULL,0,ThreadFunction,(void *)cnt,0,0);
			}
			WaitForMultipleObjects(threadCnt,threadList,TRUE,INFINITE);

			//printf("%d %lf %lf \n", i,vecEle.getReal(), vecEle.getImage());
			
			if(cnt == readCount1/2) //읽은 열수를 채운 경우
			{
				int remain = ((int)matCol+1) - colCnt; //남은 열 수 
				remain = remain*2; // 열 수 *2 : 벡터의 남은 실수와 허수
				if(remain == 0){
					//printf("%d\n",colCnt);
					//getchar();
					break;
				}
				else if(remain < (MAX_SIZE/2))//계산 하고 난 나머지 열 요소를 읽어냄
				{
					readCount1=fread(buf.n1Buffer,sizeof(double), remain, n1Matrix);
					readCount0=fread(buf.mnBuffer,sizeof(double), (readCount1*2), mnMatrix);
				}
				else if(remain >= (MAX_SIZE/2)){
					readCount1=fread(buf.n1Buffer,sizeof(double), (MAX_SIZE/2), n1Matrix);
					readCount0=fread(buf.mnBuffer,sizeof(double), (readCount1*2), mnMatrix);
				}
				colCnt += readCount1/2;		
				cnt=0;
			}
		}
		//index1=0;
	

		_fseeki64(n1Matrix, 0, SEEK_SET);		
		//파일 쓰기 
		fprintf(outText,"%d %lf %lf \n", i,vecEle.getReal(), vecEle.getImage());
		printf("%d %lf %lf \n", i,vecEle.getReal(), vecEle.getImage());
		
		double real = vecEle.getReal();
		double image = vecEle.getImage();
		fwrite(&real,sizeof(double),1,vecOut);
		fwrite(&image,sizeof(double),1,vecOut);
		vecEle.setImage(0.0);
		vecEle.setReal(0.0);
	}
	
	end_time_measurement();

	fclose(mnMatrix);
	fclose(n1Matrix);
	fclose(outText);
	fclose(vecOut);
	CloseHandle(hMutex);
	return 0;
}

unsigned int WINAPI ThreadFunction(void *arg)
{
	int matElement = (int)arg;
	int pos0 = 4*matElement+2;
	int pos1 = 2*matElement;
	Complex T1(buf.mnBuffer[pos0],buf.mnBuffer[pos0+1]);
	Complex T2(buf.n1Buffer[pos1],buf.n1Buffer[pos1+1]);
	Complex T3 = T1 * T2;

	WaitForSingleObject(hMutex, INFINITE);	//critical section 
	vecEle = vecEle + T3;
	ReleaseMutex(hMutex);

	return 0;
}
