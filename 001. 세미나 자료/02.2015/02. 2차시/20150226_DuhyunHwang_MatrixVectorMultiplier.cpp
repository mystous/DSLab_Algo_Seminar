#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "TimeMeasurement.h"
#include "MatrixVectorMultiplier.h"

double buf;
int vector_print_flag = false;
int exit_flag = false;
int detect_flag = false;

int main(){

	//int index;
	FILE_POINTERS fp;
	MATRIX matrix;
	VECTOR vector, result;
	result.real = 0;
	result.imag = 0;


	//start a timer for measurment of process time
	start_time_measurement();

	//file open
	fp.matrix = fopen("matrix.dat", "rb");
	fp.vector = fopen("vector_input.dat", "rb");
	fp.result = fopen("result_multiply.dat", "wb");

	//find matrix sizs(m x n)
	find_matrix_size(&matrix, &fp);
	printf("%f X %f\n", matrix.size.m, matrix.size.n);



	//main logic
	while (!feof(fp.matrix)){
		read_matrix(&matrix, &fp);
		//printf("%f, %f, %f, %f\n", matrix.row, matrix.column, matrix.real, matrix.imag);
		if (exit_flag == true) break;
		read_vector(&vector, &fp);
		//printf("%f, %f\n", vector.real, vector.imag);
		calc(&matrix, &vector, &result);
		//printf("%f, %f\n", result.real, result.imag);
		if (detect_flag == true)	write_result(&result, &fp);
	}


	//terminate a timer
	end_time_measurement();

	//printing
	//print_matrix(&matrix, &fp);
	//print_vector(&vector, &fp);
	//print_result(&result, &fp);


	//close file pointers and terminate process
	process_exit(&fp);

	return 0;
}


void read_matrix(MATRIX *matrix, FILE_POINTERS *fp){
	fread(&matrix->row, 1, 8, fp->matrix);
	fread(&matrix->column, 1, 8, fp->matrix);
	fread(&matrix->real, 1, 8, fp->matrix);
	fread(&matrix->imag, 1, 8, fp->matrix);

	if (feof(fp->matrix)){
		exit_flag = true;
		//process_exit(fp);
	}
	else{
		if (detect_flag == true){
			if (matrix->column != 0){
				printf("There is a error\n");
				exit_flag = true;
			}
			detect_flag = false;
		}
	}
}
void read_vector(VECTOR *vector, FILE_POINTERS *fp){
	fread(&vector->real_temp, 1, 8, fp->vector);
	fread(&vector->imag_temp, 1, 8, fp->vector);
	//fread시 마지막에 두번쓰이는 것 필터링 -> 속력저하시킴
	if (fread(&buf, 1, 1, fp->vector) == NULL){
		if (vector_print_flag == false){
			rewind(fp->vector);
			detect_flag = true;
		}
	}
	else{
		fseek(fp->vector, -1, SEEK_CUR);
	}
}

void read_result(VECTOR *result, FILE_POINTERS *fp){
	fread(&result->real_temp, 1, 8, fp->result);
	fread(&result->imag_temp, 1, 8, fp->result);
	//fread시 마지막에 두번쓰이는 것 필터링		
	if (fread(&buf, 1, 1, fp->result) == NULL){

	}
	else{
		fseek(fp->result, -1, SEEK_CUR);
	}
}

void write_result(VECTOR *result, FILE_POINTERS *fp){
	fwrite(&result->real, 1, 8, fp->result);
	fwrite(&result->imag, 1, 8, fp->result);
}

void calc(MATRIX *matrix, VECTOR *vector, VECTOR *result){
	/*(M_real + iM_imag)*(V_real + iV_imag)
	real = M_real*V_real - M_imag*V_imag
	imag = M_real*V_imag + M_imag*V_real*/
	result->real_temp = matrix->real * vector->real_temp - matrix->imag * vector->real_temp;
	result->imag_temp = matrix->real * vector->imag_temp + matrix->imag * vector->imag_temp;
	result->real += result->real_temp;
	result->imag += result->imag_temp;
}

void process_exit(FILE_POINTERS *fp){
	fclose(fp->matrix);
	fclose(fp->vector);
	fclose(fp->result);
	//end_time_measurement();
	exit(1);
}

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
		printf("%f, %f\n", vector->imag_temp, vector->imag_temp);
	}
	vector_print_flag = false;
}

void print_result(VECTOR *result, FILE_POINTERS *fp){
	fclose(fp->result);
	fp->result = fopen("result_multiply.dat", "rb");
	while (!feof(fp->result)){
		read_result(result, fp);
		printf("%f, %f\n", result->imag_temp, result->imag_temp);
	}
}

void find_matrix_size(MATRIX *matrix, FILE_POINTERS *fp){
	fseek(fp->matrix, -32, SEEK_END);
	fread(&matrix->size.m, 1, 8, fp->matrix);
	matrix->size.m++;
	fread(&matrix->size.n, 1, 8, fp->matrix);
	matrix->size.n++;
	fseek(fp->matrix, 0, SEEK_SET);
}