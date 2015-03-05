

#include "stdafx.h"
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>
#include "MatrixVectorMultiplier.h"
#include "TimeMeasurement.h"

#define _CRT_SECURE_NO_WARNINGS
#define SIZE_DOUBLE 8

POINTERS fp;
SIZES size;
BUF buf;
P_IDX p_idx;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]){
	pthread_t *threads = NULL;
	void *(*func_p)(void *);

	//pthread_t threads;
	//int thr_num = 32; //thread수 받기 설정해야함
	int thr_num;
	int status;
	
	if (argc == 5){
		size.matrix.row = atof(argv[1]);
		size.matrix.col = atof(argv[3]);
		thr_num = atoi(argv[4]);
	}
	else{
		printf("please input options");
		printf("ex) .exe m x n thread_num");
		exit(1);
	}

	//file open
	open_file(&fp);

	//find file size
	find_size(&fp, &size);

	//detect error
	detect_error(&fp, &size);

	//*****************read data**************************//
	read_data(&fp, &size, &buf);
		
	//****************read by threads*******************// ==> 실패!!
	/*free(buf.matrix);
	free(buf.vector);
	buf.matrix = (double *)malloc(sizeof(double) * size.matrix.elem_num * 4);
	buf.vector = (double *)malloc(sizeof(double) * size.vector.elem_num * 2);
	func_p = thread_read;
	make_thread(&threads, thr_num, func_p);
	join_thread(&threads, thr_num, &status); */

	//*****************calc data**************************//
	calc_data(&fp, &size, &buf);
	free(buf.output);
	//*****************calc by threads******************//
	func_p = thread_calc;
	make_thread(&threads, thr_num, func_p);
	join_thread(&threads, thr_num, &status);

	//txt 출력
	FILE *fp_txt;
	fopen_s(&fp_txt, "output.txt", "w");
	for (int i = 0; i < size.output.elem_num; i++){
		fprintf(fp_txt, "%d	:	%f	%f\n", i, buf.output[2 * i], buf.output[2 * i + 1]);
	}
	fclose(fp_txt);


	//free alloc
	free(threads);
	free_alloc(&buf);
	//exit file
	exit_process(&fp);

	return 0;
}

void open_file(POINTERS *fp){
	fopen_s(&fp->matrix, "matrix.dat", "rb");
	fopen_s(&fp->vector, "vector_input.dat", "rb");
	fopen_s(&fp->output, "vector_output.dat", "wb");
	if (fp->matrix == NULL || fp->vector == NULL || fp->output == NULL){
		printf("file open error\n");
		exit_process(fp);
	}
}

void exit_process(POINTERS *fp){
	fclose(fp->matrix);
	fclose(fp->vector);
	fclose(fp->output);
	printf("exit\n");
	exit(1);
}

void find_size(POINTERS *fp, SIZES* size){
	double buf[10];
	//******matrix size*****//
	//file size
	if (_fseeki64(fp->matrix, 0, SEEK_END) == -1){
		printf("error\n");
	}
	size->matrix.file_size = _ftelli64(fp->matrix);
	//row, col
	if (_fseeki64(fp->matrix, -SIZE_DOUBLE * 4, SEEK_END) == -1){
		printf("error\n");
	}
	fread(buf, sizeof(double), 4, fp->matrix);
	size->matrix.row = buf[0] + 1; // 1999 + 1 = 2000
	size->matrix.col = buf[1] + 1; // 99 + 1 = 100
	size->matrix.elem_num = size->matrix.row * size->matrix.col;

	//*******vector size*****//
	//file size
	if (_fseeki64(fp->vector, 0, SEEK_END) == -1){
		printf("error\n");
	}
	size->vector.file_size = _ftelli64(fp->vector);
	//row, col
	size->vector.row = size->vector.file_size / (sizeof(double) * 2);
	size->vector.col = 1;
	size->vector.elem_num = size->vector.row * size->vector.col;

	//***********output size**********//
	size->output.row = size->matrix.row;
	size->output.col = size->vector.col;
	size->output.elem_num = size->output.row * size->output.col;
	size->output.file_size = size->output.elem_num * 2;
}

void detect_error(POINTERS *fp, SIZES* size){
	//detect dimension
	if (size->matrix.col != size->vector.row){
		printf("Dimension error\n");
		exit_process(fp);
	}

	//detect vacant elements
	if (size->matrix.file_size / (sizeof(double) * 4) != size->matrix.row * size->matrix.col){
		printf("Element error\n");
		exit_process(fp);
	}
}

void read_data(POINTERS *fp, SIZES* size, BUF *buf){
	// read data
	start_time_measurement();
	buf->matrix = (double *)malloc(sizeof(double) * size->matrix.elem_num * 4);
	_fseeki64(fp->matrix, 0, SEEK_SET);
	fread(buf->matrix, sizeof(double), size->matrix.elem_num * 4, fp->matrix);
	buf->vector = (double *)malloc(sizeof(double) * size->vector.elem_num * 2);
	_fseeki64(fp->vector, 0, SEEK_SET);
	fread(buf->vector, sizeof(double), size->vector.elem_num * 2, fp->vector);
	end_time_measurement();
}

void calc_data(POINTERS *fp, SIZES* size, BUF *buf){
	buf->output = (double *)malloc(sizeof(double) * size->output.elem_num * 2);
	memset(buf->output, 0, sizeof(double) * size->output.elem_num * 2);
	start_time_measurement();
	int matrix_idx, vector_idx, output_idx;
	matrix_idx = 0;
	for (output_idx = 0; output_idx < size->output.elem_num; output_idx++){
		for (vector_idx = 0; vector_idx < size->vector.elem_num; vector_idx++){
			buf->output[2 * output_idx] += real_calc(buf->matrix[4 * matrix_idx + 2], buf->matrix[4 * matrix_idx + 3],
				buf->vector[2 * vector_idx], buf->vector[2 * vector_idx + 1]);
			buf->output[2 * output_idx + 1] += imag_calc(buf->matrix[4 * matrix_idx + 2], buf->matrix[4 * matrix_idx + 3],
				buf->vector[2 * vector_idx], buf->vector[2 * vector_idx + 1]);
			matrix_idx++;
		}
	}
	_fseeki64(fp->output, 0, SEEK_SET);
	fwrite(buf->output, sizeof(double), size->output.elem_num * 2, fp->output);
	end_time_measurement();
}

double real_calc(double A_real, double A_imag, double B_real, double B_imag){
	return A_real * B_real - A_imag * B_imag;
}

double imag_calc(double A_real, double A_imag, double B_real, double B_imag){
	return A_real * B_imag + A_imag * B_real;
}

void free_alloc(BUF *buf){
	free(buf->matrix);
	free(buf->vector);
	free(buf->output);
}

void make_thread(pthread_t **threads, int thr_num, void *(*func_p)(void*)){
	//start measurement

	int thr_id;
	*threads = (pthread_t *)malloc(sizeof(pthread_t)*thr_num);
	p_idx.pt = (PT *)malloc(sizeof(PT)*thr_num);
	p_idx.m_quotient = size.matrix.row / thr_num;
	p_idx.n_quotient = size.vector.row / thr_num;
	buf.output = (double *)malloc(sizeof(double) * size.output.elem_num * 2);	//할당
	memset(buf.output, 0, sizeof(double) * size.output.elem_num * 2);	//초기화


	start_time_measurement();	//측정
	
	
	for (int i = 0; i < thr_num; i++){
		p_idx.pt[i].m_start_idx = i*p_idx.m_quotient;
		p_idx.pt[i].n_start_idx = i*p_idx.n_quotient;
		if (i != thr_num){
			p_idx.pt[i].m_end_idx = p_idx.pt[i].m_start_idx + p_idx.m_quotient - 1;
			p_idx.pt[i].n_end_idx = p_idx.pt[i].n_start_idx + p_idx.n_quotient - 1;
		}
		else{
			p_idx.pt[i].m_end_idx = size.output.elem_num - 1;//나누어 남는 부분은 부분은 마지막 thread가 모두 처리한다.
			p_idx.pt[i].n_end_idx = size.vector.elem_num - 1;//나누어 남는 부분은 부분은 마지막 thread가 모두 처리한다.
		}
		thr_id = pthread_create(*threads + i, NULL, func_p, (void *)&p_idx.pt[i]); //인자 전달부분에 계산 시작point를 전달
		if (thr_id < 0){
			perror("thread create error : ");

		}
		//printf("Thread %d, %d\n", i, *(*threads+i));
	}
}

void join_thread(pthread_t **threads, int thr_num, int *status){
	int i;
	for (i = 0; i < thr_num; i++){
		pthread_join(*(*threads + i), NULL);
	}
	//write
	_fseeki64(fp.output, 0, SEEK_SET);
	fwrite(buf.output, sizeof(double), size.output.elem_num * 2, fp.output);
	end_time_measurement(); //측정 종료
}

void *thread_read(void *pt){
	

	pthread_mutex_lock(&mutex);
	_fseeki64(fp.matrix, ((PT *)pt)->m_start_idx * 4, SEEK_SET);
	fread(&buf.matrix[(int)((PT *)pt)->m_start_idx * 4], sizeof(double), (((PT *)pt)->m_end_idx - ((PT *)pt)->m_start_idx + (double)1) * 4, fp.matrix);
	printf("바보바보\n");
	_fseeki64(fp.vector, ((PT *)pt)->n_start_idx * 2, SEEK_SET);
	fread(&buf.matrix[(int)((PT *)pt)->n_start_idx * 2], sizeof(double), (((PT *)pt)->n_end_idx - ((PT *)pt)->n_start_idx + (double)1) * 2, fp.vector);
	pthread_mutex_unlock(&mutex);
	pthread_exit((void *)0);
	return 0;
}

void *thread_calc(void *pt){	int matrix_idx, vector_idx, output_idx;
	matrix_idx = ((PT *)pt)->m_start_idx;
	for (output_idx = ((PT *)pt)->m_start_idx; output_idx <= ((PT *)pt)->m_end_idx; output_idx++){
		for (vector_idx = 0; vector_idx < size.vector.elem_num; vector_idx++){
			buf.output[2 * output_idx] += real_calc(buf.matrix[4 * matrix_idx + 2], buf.matrix[4 * matrix_idx + 3],
				buf.vector[2 * vector_idx], buf.vector[2 * vector_idx + 1]);
			buf.output[2 * output_idx + 1] += imag_calc(buf.matrix[4 * matrix_idx + 2], buf.matrix[4 * matrix_idx + 3],
				buf.vector[2 * vector_idx], buf.vector[2 * vector_idx + 1]);
			matrix_idx++;
		}
	}
	pthread_exit((void *)0);	return 0;}
