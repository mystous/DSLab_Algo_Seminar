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
#define THREAD_N 32


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
HANDLE fMutex;

FILE *mnMatrix;
FILE *n1Matrix;
FILE *outText;
FILE *vecOut;
double matRow=0, matCol=0;

int _tmain(int argc, _TCHAR* argv[])
{
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
	int matRowCnt=0;
	while( matRowCnt < (double)(matRow+1) )
	{
		int threadCnt=0;
		HANDLE threadList[THREAD_N];
		for(matRowCnt ; matRowCnt < (double)(matRow+1) && threadCnt<THREAD_N ; matRowCnt++, threadCnt++)
		{
			
			threadList[threadCnt] = (HANDLE)_beginthreadex(NULL,0,ThreadFunction,(void *)matRowCnt, 0, 0);
		}
	
		WaitForMultipleObjects(threadCnt, threadList, TRUE, INFINITE);
		
		for(int i=0 ; i<THREAD_N ; i++)
		{
			CloseHandle(threadList[i]);
		}
				
		//printf("matRowCnt %d\n", matRowCnt);
		
	}
	
	end_time_measurement();

	fclose(mnMatrix);
	fclose(n1Matrix);
	fclose(outText);
	fclose(vecOut);
	CloseHandle(hMutex);
	CloseHandle(fMutex);
	return 0;
}

unsigned int WINAPI ThreadFunction(void *arg)
{
	int rowCnt = (int)arg;
	double mnBuffer[MAX_SIZE];
	double n1Buffer[MAX_SIZE/2];
	Complex T1, T2, T3, T4(0,0);
	int matElement=0;
	int posMn, posN1;
	int rowPoint = rowCnt * sizeof(double) * 4 * ((int)matCol+1);
	int sumReadCnt1=0;
	int sumReadCnt0=0;

	
	int index0=0;
	int index1=0;
	while(matElement < (int)(matCol+1)){
		//CRITICAL SECTION
		WaitForSingleObject(hMutex, INFINITE);	//critical section 
		_fseeki64(mnMatrix, rowPoint+index0, SEEK_SET);
		_fseeki64(n1Matrix, index1, SEEK_SET);
		int readCount1 = fread(n1Buffer,sizeof(double),MAX_SIZE/2,n1Matrix) ;
		int readCount0 = fread(mnBuffer,sizeof(double),readCount1*2,mnMatrix);
		ReleaseMutex(hMutex);
		///////////////////
	
		sumReadCnt1+=readCount1;//읽은 열  
		sumReadCnt0+=readCount0;//읽은 열

		int col=0;
		for( matElement; (matElement < sumReadCnt1/2) && matElement < (int)(matCol+1) ; matElement++,col++) // 계산
		{
			posMn = 4*col+2;
			posN1 = 2*col;
			T1.setReal(mnBuffer[posMn]); T1.setImage(mnBuffer[posMn+1]);
			T2.setReal(mnBuffer[posN1]); T2.setImage(mnBuffer[posN1+1]);
			T3 = T1 * T2;
			T4 = T4 + T3;
		}

		index0=sumReadCnt0*sizeof(double);
		index1=sumReadCnt1*sizeof(double);

	}	
	//printf("%d %lf %lf \n",rowCnt, T4.getReal(), T4.getImage());
	//printf("thread 탈출\n");
	
	WaitForSingleObject(fMutex, INFINITE);	//critical section 
	double real = T4.getReal();
	double image = T4.getImage();
	fwrite(&real,sizeof(double),1,vecOut);
	fwrite(&image,sizeof(double),1,vecOut);
	fprintf(outText,"%d %lf %lf \n",rowCnt, T4.getReal(), T4.getImage());
	ReleaseMutex(fMutex);

	return 0;
}
