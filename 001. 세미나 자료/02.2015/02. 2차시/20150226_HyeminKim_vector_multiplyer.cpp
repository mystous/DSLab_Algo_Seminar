#include <stdio.h>
#include <stdlib.h>
#include "TimeMeasurement.h"


#define MAXSIZE 1024

typedef struct cnum
{
	//double row;
	//double col;
	double realnum;
	double imanum;
}Cnum;

Cnum mult(Cnum num1,Cnum num2)
{
	Cnum result;
	result.realnum = (num1.realnum*num2.realnum)-(num1.imanum*num2.imanum);
	result.imanum = (num1.realnum*num2.imanum)+(num1.imanum*num2.realnum);

	return result;
}


Cnum sum(Cnum num1,Cnum num2)
{
	Cnum result;
	result.realnum = num1.realnum+num2.realnum;
	result.imanum = num1.imanum+num2.imanum;

	return result;
}

int main(void)
{
	FILE	*mat;
	FILE	*vec;
	FILE	*end;

	double	main_buf[MAXSIZE];
	double  main_vec_buf[MAXSIZE];
	Cnum	mat_buf[MAXSIZE];
	Cnum	vec_buf[MAXSIZE];
	Cnum	result[MAXSIZE];

	double	Row=0, Col=0;
	double	vec_row;

	double  rc_arr[2];

	long mat_in=0;

	mat = fopen("matrix.dat","rb");
	vec = fopen("vector_input.dat","rb");
	end = fopen("vector_output.dat","wb");

	// ���� ���� �˻�

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

	start_time_measurement(); //�ð����� ����

	// ��Ʈ���� ũ��
	int point;
	point = -4*sizeof(double);

	if(fseek(mat, point, SEEK_END)==-1)
	{
		printf("error\n");
	}

	fread(rc_arr,sizeof(double),2,mat);

	Row=rc_arr[0]+1;
	Col=rc_arr[1]+1;

	printf("��ü ���� �� = %f\n��ü ���� �� = %f \n", Row, Col);

	//vec ��� �� ũ��
	fseek(vec,0,SEEK_END);
	vec_row = ftell(vec)/(sizeof(double)*2);
	printf("������� �� �� = %f\n",vec_row);

	//��� ũ�Ⱑ �ٸ� �� ���� �޼��� ���
	if(Col!=vec_row)
	{
		fputs("[ERROR] Matrix and Vector are not same", stderr);
		exit(1);
	}

	//��İ� ������ ����
	int j=0;
	int button=0;

	long fpos;


	fseek(mat,0,SEEK_SET); //���� ������ ��ġ �ʱ�ȭ
	fseek(vec,0,SEEK_SET); //���� ������ ��ġ �ʱ�ȭ
	fseek(end,0,SEEK_SET); //���� ������ ��ġ �ʱ�ȭ

	for(int k=0; k<2000; k++)
	{
		//�ε��� �ʱ�ȭ
		button =0;
		j=0;
		fseek(vec,0,SEEK_SET); //���� ������ ��ġ �ʱ�ȭ

		//mat ��� �����ͼ� mat_buf[]�� ���� 100���� -> ////vec ���� ������ ����� �����ؾ��� //// �ϴ��� 100��������
		for(int i=0; i<400; i++)
		{
			fread(&main_buf[i], sizeof(double), 1, mat);
			//printf("�� : %f ", main_buf[i]);

			if(i>3 && i%4==0)
				j++;

			// ����ü�� ������
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
	
		/* mat index üũ ,�� üũ
	mat_in = ftell(mat);
	printf("mat �ε��� �� = %d\n",mat_in);
		
	for(int i=0;i<100;i++)
		printf("realnum : %f imanum : %f \n",mat_buf[i].realnum,mat_buf[i].imanum);

		*/

		//vec ��� �����ͼ� vec_buf[]�� ����
		j=0;

		for(int y=0; y<200; y++)
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
			
			//printf("vec [%d] = �� : %f\n ",y, main_vec_buf[y]);

		}
		


		// mat * vec

		for(int z=0; z<100; z++)
		{
			result[z]=mult(mat_buf[z],vec_buf[z]);
			//printf("result [%d] : %f, %f\n",z,result[z].realnum,result[z].imanum);
		}

		for(int zy=1; zy<100; zy++)
		{
			result[0]=sum(result[0],result[zy]);			
		}


		//��������� ��� �̼��� fwrite �ȴ�
		//printf("vec real [0] : %f ",result[0].realnum);
		//printf("vec ima  [0] : %f\n",result[0].imanum);

		fwrite(&result[0].realnum,sizeof(double),1,end);
		fwrite(&result[0].imanum,sizeof(double),1,end);

		printf("[%d] realnum : %f imanum : %f \n",k,result[0].realnum,result[0].imanum);

		
		/*
		fclose(end);

		end = fopen("vector_output.dat","rb");
		double bbb[2];
		fread(&bbb[0],sizeof(double),1,end);
		fread(&bbb[1],sizeof(double),1,end);

		//printf("realnum : %f imanum : %f \n",bbb[0],bbb[1]);
		*/
		


	}

	/*
	for(int i=0;i<100;i++)
	{
	//vec_buf �� ǥ��
	printf("vec real [%d] : %f ",i,vec_buf[i].realnum);
	printf("vec ima  [%d] : %f\n",i,vec_buf[i].imanum);

	}
	*/

	end_time_measurement(); //�ð� ���� ��

	fclose(mat);
	fclose(vec);
	fclose(end);


	return 0;
}

/*
int mat_index=3;

fseek(mat,1,SEEK_SET); //���� ������ ��ġ �ʱ�ȭ

int fpos;
fpos=ftell(mat);
printf("fpos = %d\n",fpos);

fseek(mat,3,SEEK_SET); //���� ������ ��ġ �ʱ�ȭ

fread(&main_buf[0], sizeof(double), 2, mat);
printf("main_buf[0] = %f \n",main_buf[0]);
fpos=ftell(mat);
printf("fpos = %d\n",fpos);

fseek(mat,4,SEEK_SET);

fread(&main_buf[1], sizeof(double), 2, mat);
printf("main_buf[1] = %f \n",main_buf[1]);
fpos=ftell(mat);
printf("fpos = %d\n",fpos);
*/

