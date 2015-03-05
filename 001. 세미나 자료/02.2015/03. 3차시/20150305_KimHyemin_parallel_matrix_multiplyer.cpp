#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "TimeMeasurement.h"


#define MAXSIZE 8192

typedef struct cnum
{
	double realnum;
	double imanum;
}Cnum;
/*
Cnum mult(Cnum num1,Cnum num2)
{
Cnum result;
result.realnum = (num1.realnum*num2.realnum)-(num1.imanum*num2.imanum);
result.imanum = (num1.realnum*num2.imanum)+(num1.imanum*num2.realnum);

return result;
}
*/
/*
Cnum sum(Cnum num1,Cnum num2)
{
Cnum result;
result.realnum = num1.realnum+num2.realnum;
result.imanum = num1.imanum+num2.imanum;

return result;
}
*/

void mult_mat(Cnum* mat, Cnum* vec, Cnum* result, int index, int thread)
{
	omp_set_num_threads(thread);
#pragma omp parallel 
	{
#pragma omp for
		for(int i=0; i<index;i++)
		{

			result[i].realnum = (mat[i].realnum*vec[i].realnum)-(mat[i].imanum*vec[i].imanum);
			result[i].imanum  = (mat[i].realnum*vec[i].imanum)+(mat[i].imanum*vec[i].realnum);


			//printf("%d  %d  %f  %f\n",omp_get_thread_num(),i,result[i].realnum, result[i].imanum);

		}

	}

}
void sum_mat(Cnum* result, int index)
{


	for(int i=1; i<index; i++)
	{
		result[0].realnum += result[i].realnum;
		result[0].imanum += result[i].imanum;
	}

}

/*
void sum_mat(Cnum* final_result, Cnum* result, int index)
{
#pragma omp parallel
{
#pragma omp for
for(int i=0; i<index; i++)
{
//printf("Thread num= %d  %d  %f   %f \n ",omp_get_thread_num(),i,result[0].realnum,result[0].imanum);
final_result[0].realnum+= result[i].realnum;
final_result[0].imanum+= result[i].imanum;
}
}

}
*/







int main(int argc, char *argv[])
{
	FILE	*mat;
	FILE	*vec;
	FILE	*end;

	double	main_buf[MAXSIZE];
	double  main_vec_buf[MAXSIZE];
	Cnum	mat_buf[MAXSIZE];
	Cnum	vec_buf[MAXSIZE];
	Cnum	result[MAXSIZE];
	Cnum	final_result[2]={{0.0,0.0},{0.0,0.0}};

	double	Row=0, Col=0;
	double	vec_row;

	double  mat_RC_data[6];

	long	mat_in=0;

	int		j=0;
	int		thread=atoi(argv[3]);

	mat = fopen("matrix.dat","rb");
	vec = fopen("vector_input.dat","rb");
	end = fopen("vector_output.dat","wb");

	
	// exe input number 검사
	if (argc == 4 && atoi(argv[1]) && atoi(argv[2]))
	printf("input = %d x %d, Thread = %d \n",atoi(argv[1]), atoi(argv[2]),atoi(argv[3]));
	else
	{
	fputs("[Error] Not a correct input", stderr);
	exit(1);
	}
	

	// 파일 유무 검사

	if(mat==NULL)
	{
		fputs("Matrix File error", stderr);
		exit(1);
	}

	if(vec==NULL)
	{
		fputs("Vector File error", stderr);
		exit(1);
	}

	start_time_measurement(); //시간측정 시작

	// 매트릭스 크기

	int point;

	for(int i=3;i>0;i--)
	{
		point = i*4*sizeof(double);
		point = -point;
		if(fseek(mat, point, SEEK_END)==-1)
		{
			printf("error\n");
		}
		fread(&mat_RC_data[j],sizeof(double),2,mat);
		Row=mat_RC_data[j]+1;
		j++;
		Col=mat_RC_data[j]+1;
		j++;
		printf("row : %f, col : %f\n",Row,Col);
	}

	/*
	if(( (mat_RC_data[0]) != (mat_RC_data[2]) != (mat_RC_data[4]) ))
	{
	fputs("[ERROR] Matrix File ROW_DATA error", stderr);
	exit(1);
	}
	if(mat_RC_data[1]+2!=mat_RC_data[3]+1!=mat_RC_data[5])
	{
	fputs("[ERROR] Matrix File COL_DATA error", stderr);
	exit(1);
	}
	*/

	// matrix.dat 행 열값 부분 오류 검사
	if(mat_RC_data[4]-mat_RC_data[2]>1)
	{
		fputs("[ERROR] Matrix File ROW_DATA error", stderr);
		exit(1);
	}
	if(mat_RC_data[2]-mat_RC_data[0]>1)
	{
		fputs("[ERROR] Matrix File ROW_DATA error", stderr);
		exit(1);
	}
	if(mat_RC_data[5]-mat_RC_data[3]>2)
	{
		fputs("[ERROR] Matrix File ROW_DATA error", stderr);
		exit(1);
	}
	if(mat_RC_data[3]-mat_RC_data[1]>2)
	{
		fputs("[ERROR] Matrix File ROW_DATA error", stderr);
		exit(1);
	}

	printf("전체 열의 값 = %f\n전체 행의 값 = %f \n", Row, Col);

	//vec 행렬 행 크기
	fseek(vec,0,SEEK_END);
	vec_row = ftell(vec)/(sizeof(double)*2);
	printf("벡터행렬 행 값 = %f\n",vec_row);

	//행렬 크기가 다를 시 오류 메세지 출력
	if(Col!=vec_row)
	{
		fputs("[ERROR] Matrix and Vector are not same", stderr);
		exit(1);
	}

	//행렬값 꺼내서 연산
	j=0;
	int button=0;

	fseek(mat,0,SEEK_SET); //파일 포인터 위치 초기화
	fseek(vec,0,SEEK_SET); //파일 포인터 위치 초기화
	fseek(end,0,SEEK_SET); //파일 포인터 위치 초기화

	for(int k=0; k<Row; k++)
	{
		//인덱스 초기화
		button =0;
		j=0;
		fseek(vec,0,SEEK_SET); //파일 포인터 위치 초기화

		//mat 행렬 가져와서 mat_buf[]에 저장 100개씩 -> ////vec 행의 갯수에 따라로 수정해야함 //// 일단은 100개단위로
		for(int i=0; i<Col*4; i++)
		{
			fread(&main_buf[i], sizeof(double), 1, mat);
			//printf("값 : %f ", main_buf[i]);

			if(i>3 && i%4==0)
				j++;

			// 구조체에 값저장
			if(button==2)
				mat_buf[j].realnum=main_buf[i];
			else if(button==3)
			{
				mat_buf[j].imanum=main_buf[i];
			}
			button++;
			if(button==4)
				button=0;
		}

		//vec 행렬 가져와서 vec_buf[]에 저장
		j=0;

		for(int y=0; y<Col*2; y++)
		{
			fread(&main_vec_buf[y],sizeof(double),1,vec);
			if(y>1&&y%2==0)
				j++;
			if(y%2==0)
				vec_buf[j].realnum = main_vec_buf[y];
			else
			{
				vec_buf[j].imanum = main_vec_buf[y];
			}			

		}




		// mat * vec
		/*
		for(int i=0; i<Col; i++)
		result[i]=mult(mat_buf[i],vec_buf[i]); 
		*/

		mult_mat(mat_buf,vec_buf,result,(int)Col,thread);
		sum_mat(result,(int)Col);

		/*
		for(int i=1; i<Col; i++)
		{
		result[0]=sum(result[0],result[i]);			
		}
		*/


		fwrite(&result[0].realnum,sizeof(double),1,end);
		fwrite(&result[0].imanum,sizeof(double),1,end);

		//printf("%f, %f \n",final_result[0].realnum,final_result[0].imanum);

	}
	printf("Calculation Complete ");
	end_time_measurement(); //시간 측정 끝

	fclose(mat);
	fclose(vec);
	fclose(end);


	return 0;
}
/*
// exe input number 검사
if (argc == 4 && atoi(argv[1]) && atoi(argv[2]))
printf("input = %d x %d, Thread = %d \n",atoi(argv[1]), atoi(argv[2]),atoi(argv[3]));
else
{
fputs("[Error] Not a correct input", stderr);
exit(1);
}
*/
