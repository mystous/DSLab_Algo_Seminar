#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "TimeMeasurement.h"
#include "MatrixVectorMultiplier.h"

double buf = 0;
//int sampling_rate = 60;
long number_of_vector = 0;

bool exit_flag = false;
bool vector_end_flag = false;
bool vector_firstend_flag = true;
bool detect_flag = false;

bool vector_print_flag = false;

int main(){

	FILE_POINTERS fp;
	MATRIX matrix;
	VECTOR vector, result;
	result.writing_real = 0;
	result.writing_imag = 0;
	
	//start a timer for measurment of process time
	start_time_measurement();

	//file open
	fp.matrix = fopen("matrix.dat", "rb");
	fp.vector = fopen("vector_input.dat", "rb");
	fp.result = fopen("result_multiply.dat", "wb");
	
	//file open error
	if (fp.matrix == NULL || fp.vector == NULL || fp.result == NULL){
		printf("file open error\n");
		return 0;
	}

	//find matrix sizs(m x n)
	find_matrix_size(&matrix, &fp);
	printf("%f X %f\n", matrix.size.m, matrix.size.n);

	//decide a sampling rate for detecting
	//-> 샘플링을 해도 수행시간이 줄지 않을 것으로 생각됨

	//main logic
	while (!feof(fp.matrix)){
		read_matrix(&matrix, &fp);
		if (exit_flag == true) break;
		if (vector_end_flag == true)	detect_error(&matrix);
		read_vector(&vector, &fp);
		calc(&matrix, &vector, &result);
		if (vector_end_flag == true)	write_result(&result, &fp);
	}


	//terminate a timer
	end_time_measurement();
	
	//printing
	//print_matrix(&matrix, &fp);
	//print_vector(&vector, &fp);
	//print_result(&result, &fp);
	//print_matrix_front(&matrix, &fp, 4);
	//print_matrix_back(&matrix, &fp, 10);
	//print_vector_front(&vector, &fp, 4);
	//print_vector_back(&vector, &fp, 10);
	//print_result_front(&result, &fp, 4);
	//print_result_back(&result, &fp, 10);
	
	//close file pointers and terminate process
	process_exit(&fp);

	return 0;
}




void read_matrix(MATRIX *matrix, FILE_POINTERS *fp){
	//save a previous row
	matrix->prev_row = matrix->row;
	//read
	fread(&matrix->row, 1, 8, fp->matrix);
	fread(&matrix->column, 1, 8, fp->matrix);
	fread(&matrix->real, 1, 8, fp->matrix);
	fread(&matrix->imag, 1, 8, fp->matrix);

	if (feof(fp->matrix)){
		exit_flag = true;
	}
}
void read_vector(VECTOR *vector, FILE_POINTERS *fp){
	fread(&vector->real, 1, 8, fp->vector);
	fread(&vector->imag, 1, 8, fp->vector);
	//fread시 마지막에 두번쓰이는 것 필터링 -> 속력저하시킴
	if (fread(&buf, 1, 1, fp->vector) == NULL){
		if (vector_print_flag == false){
			if (vector_firstend_flag == true){
				number_of_vector = ftell(fp->vector)/16;
				vector_firstend_flag = false;
			}
			rewind(fp->vector);
			vector_end_flag = true;
		}
	}
	else{
		fseek(fp->vector, -1, SEEK_CUR);
	}
}

void read_result(VECTOR *result, FILE_POINTERS *fp){
	fread(&result->real, 1, 8, fp->result);
	fread(&result->imag, 1, 8, fp->result);
	//fread시 마지막에 두번쓰이는 것 필터링		
	if (fread(&buf, 1, 1, fp->result) == NULL){

	}
	else{
		fseek(fp->result, -1, SEEK_CUR);
	}
}

void write_result(VECTOR *result, FILE_POINTERS *fp){
	fwrite(&result->writing_real, 1, 8, fp->result);
	fwrite(&result->writing_imag, 1, 8, fp->result);
	result->writing_real = 0;
	result->writing_imag = 0;
}

void calc(MATRIX *matrix, VECTOR *vector, VECTOR *result){
	/*(M_real + iM_imag)*(V_real + iV_imag)
	real = M_real*V_real - M_imag*V_imag
	imag = M_real*V_imag + M_imag*V_real*/
	result->real = matrix->real * vector->real - matrix->imag * vector->imag;
	result->imag = matrix->real * vector->imag + matrix->imag * vector->real;
	result->writing_real += result->real;
	result->writing_imag += result->imag;
}

void process_exit(FILE_POINTERS *fp){
	fclose(fp->matrix);
	fclose(fp->vector);
	fclose(fp->result);
	exit(1);
}

void find_matrix_size(MATRIX *matrix, FILE_POINTERS *fp){
	fseek(fp->matrix, -32, SEEK_END);
	fread(&matrix->size.m, 1, 8, fp->matrix);
	matrix->size.m++;
	fread(&matrix->size.n, 1, 8, fp->matrix);
	matrix->size.n++;
	fseek(fp->matrix, 0, SEEK_SET);
}

int detect_error(MATRIX *matrix){
	if (vector_end_flag == true){
		//차원일치여부
		if (matrix->size.n != number_of_vector){
			printf("different dimension\n");
			exit_flag = true;
			vector_end_flag = false;
			return 0;
		}
		//일부 값이 비어있을 경우
		//값이 비어 열보다 작은 만큼 비어있을 경우, column값이 0이 아니다.
		//또는 열 전체가 비어있을 경우, row값이 1이상 증가했을 것이다.
		if (matrix->prev_row + 1 < matrix->row || matrix->column != 0){
			printf("There are no several data\n");
			exit_flag = true;
			vector_end_flag = false;
			return 0;
		}
		vector_end_flag = false;
	}
	return 0;
}


////////////////////////////////////printing functions//////////////////////////////////////
void print_matrix(MATRIX *matrix, FILE_POINTERS *fp){
	fseek(fp->matrix, 0, SEEK_SET);
	while (!feof(fp->matrix)){
		read_matrix(matrix, fp);
		printf("%f, %f, %f, %f\n", matrix->row, matrix->column, matrix->real, matrix->imag);
	}

}
void print_vector(VECTOR *vector, FILE_POINTERS *fp){
	vector_print_flag = true;
	fseek(fp->vector, 0, SEEK_SET);
	while (!feof(fp->vector)){
		read_vector(vector, fp);
		printf("%f, %f\n", vector->real, vector->imag);
	}
	vector_print_flag = false;
}
void print_result(VECTOR *result, FILE_POINTERS *fp){
	fclose(fp->result);
	fp->result = fopen("result_multiply.dat", "rb");
	while (!feof(fp->result)){
		read_result(result, fp);
		printf("%f, %f\n", result->real, result->imag);
	}
}
int print_matrix_front(MATRIX *matrix, FILE_POINTERS *fp, int count){
	int index = 0;
	if (count < 1 || count > 10) return -1;
	fseek(fp->matrix, 0 * 32, SEEK_SET);
	for (index = 0; index < count; index++){
		read_matrix(matrix, fp);
		printf("%f, %f, %f, %f\n", matrix->row, matrix->column, matrix->real, matrix->imag);
	}
	return 0;
}
void print_matrix_back(MATRIX *matrix, FILE_POINTERS *fp, int count){
	int index = 0;
	fseek(fp->matrix, -count * 32, SEEK_END);
	for (index = 0; index < count; index++){
		read_matrix(matrix, fp);
		printf("%f, %f, %f, %f\n", matrix->row, matrix->column, matrix->real, matrix->imag);
	}
}
int print_vector_front(VECTOR *vector, FILE_POINTERS *fp, int count){
	int index = 0;
	if (count < 1 || count > 10) return -1;
	fseek(fp->vector, 0 * 16, SEEK_SET);
	for (index = 0; index < count; index++){
		read_vector(vector, fp);
		printf("%f, %f\n", vector->real, vector->imag);
	}
	return 0;
}
void print_vector_back(VECTOR *vector, FILE_POINTERS *fp, int count){
	int index = 0;
	fseek(fp->vector, -count * 16, SEEK_END);
	for (index = 0; index < count; index++){
		read_vector(vector, fp);
		printf("%f, %f\n", vector->real, vector->imag);
	}
}
int print_result_front(VECTOR *result, FILE_POINTERS *fp, int count){
	int index = 0;
	if (count < 1 || count > 10) return -1;
	fclose(fp->result);
	fp->result = fopen("result_multiply.dat", "rb");
	fseek(fp->result, 0 * 16, SEEK_SET);
	for (index = 0; index < count; index++){
		read_result(result, fp);
		printf("%f, %f\n", result->real, result->imag);
	}
	return 0;
}
void print_result_back(VECTOR *result, FILE_POINTERS *fp, int count){
	int index = 0;
	fclose(fp->result);
	fp->result = fopen("result_multiply.dat", "rb");
	fseek(fp->vector, -count * 16, SEEK_END);
	for (index = 0; index < count; index++){
		read_result(result, fp);
		printf("%f, %f\n", result->real, result->imag);
	}
}