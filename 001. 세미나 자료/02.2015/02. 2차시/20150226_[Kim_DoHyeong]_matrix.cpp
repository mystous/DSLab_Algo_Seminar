// matrix.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include "TimeMeasurement.h"
using namespace std;

#define MAX_SIZE 1024

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

int _tmain(int argc, _TCHAR* argv[])
{
	
	double mnBuffer[MAX_SIZE];
	double n1Buffer[MAX_SIZE];
	double matRow=0, matCol=0;
	FILE *mnMatrix;
	FILE *n1Matrix;
	FILE *outText;

	fopen_s(&mnMatrix,"matrix.dat","rb");
	fopen_s(&n1Matrix,"vector_input.dat","rb");
	fopen_s(&outText,"out.txt","w");
	start_time_measurement();

	/****************매트릭스 크기**********************/
	long int point;
	point = 4*sizeof(double);
	point = -point;
	if(fseek(mnMatrix, point, SEEK_END)==-1)
	{
		printf("오류\n");
	}


	fread(mnBuffer,sizeof(double),4,mnMatrix);
	printf("%f %f %f %f\n", mnBuffer[0], mnBuffer[1], mnBuffer[2], mnBuffer[3]);
	matRow=mnBuffer[0];
	matCol=mnBuffer[1];

	/******************* 행 렬 검 사 **************************/
	if(fseek(mnMatrix, 0, SEEK_SET)==-1)
	{
		printf("오류\n");
	}
	printf("double 크기 %d\n", sizeof(double));
    
	
	//test    //vector 개수 
	volatile int test=0;
	int result=0;
	while(result=fread(n1Buffer,sizeof(double),MAX_SIZE,n1Matrix))
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
	
	/// 행렬 값 확인
	if(fseek(mnMatrix, 0, SEEK_SET)==-1)
	{
		printf("오류\n");
	}
	long size = sizeof(double)*((long)matCol+1)*(long)matRow*4;
	//printf("%d\n", size);
	fseek(mnMatrix, size ,SEEK_SET);
	fread(mnBuffer,sizeof(double),4,mnMatrix);
	//printf("행%f, 열%f 끝행%f\n", mnBuffer[0], mnBuffer[1], matRow);
	if((mnBuffer[0] != matRow) || ((int)mnBuffer[1])!=0 )
	{
		printf("행렬안에 빠진게 있습니다.\n");
		exit(1);
	}


	/******************* 연 산 **************************/
	if(fseek(mnMatrix, 0, SEEK_SET)==-1)
	{
		printf("seek 오류\n");
	}
	if(fseek(n1Matrix, 0, SEEK_SET)==-1)
	{
		printf("seek 오류\n");
	}
	

	int readCount0=fread(mnBuffer,sizeof(double), MAX_SIZE, mnMatrix);
	int readCount1=fread(n1Buffer,sizeof(double), MAX_SIZE, n1Matrix);
	int index0=0, index1=0;
	int t=0;
	for(int i=0 ; i<(matRow+1) ; i++) // 계산하는 횟수(행수 만큼 연산)
	{
		Complex vecEle(0,0);
		int j;
		for( j=0; j<(matCol+1); j++)
		{
			int pos0 = 4*index0+2;
			int pos1 = 2*index1;
			if(((pos0+2) > readCount0) && (readCount0!=0)){
				index0=0;
				pos0 = 4*index0+2;
				readCount0=fread(mnBuffer,sizeof(double), MAX_SIZE, mnMatrix);
				//fprintf(outText,"\n %f %f %f %f\n ", mnBuffer[0], mnBuffer[1], mnBuffer[2], mnBuffer[3]);
			}
			Complex T1(mnBuffer[pos0],mnBuffer[pos0+1]);
			//fprintf(outText,"%d %d %f %f POS %d %d", j
			index0++;
				
			if(((pos1+1) > readCount1) && (readCount1!=0) ){
				index1=0;
				pos1 = 2*index1;
				readCount1=fread(n1Buffer,sizeof(double), MAX_SIZE, n1Matrix);
			}
			Complex T2(n1Buffer[pos1],n1Buffer[pos1+1]);
			index1++;		
			Complex T = T1 * T2;
			vecEle = vecEle + T;
		}
		index1=0;
		fseek(n1Matrix, 0, SEEK_SET);
		readCount1=fread(n1Buffer,sizeof(double), MAX_SIZE, n1Matrix);
		
		fprintf(outText,"%d %lf %lf \n", i,vecEle.getReal(), vecEle.getImage());
	}
	
	end_time_measurement();

	fclose(mnMatrix);
	fclose(n1Matrix);
	fclose(outText);
	return 0;
}


