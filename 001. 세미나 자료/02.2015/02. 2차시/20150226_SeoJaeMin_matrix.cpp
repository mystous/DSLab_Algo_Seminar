#include <stdio.h>
#include <stdlib.h>

typedef struct MyStruct{
	double row;
	double column;
	double real;
	double imaginary;
}COMPLEX;

COMPLEX multiplier(COMPLEX num1, COMPLEX num2);
COMPLEX adder(COMPLEX num1, COMPLEX num2);

int main(){
	
	int i,j;
	int mat_mul_pos;
	int mat_index=0;
	int vec_mul_pos;
	int vec_index=0;
	
	COMPLEX matrixBuffer[1024];
	COMPLEX vectorBuffer[1024];
	COMPLEX outputBuffer[1024];

	double matrixRow=0;
	double matrixColumn=0;

	COMPLEX output_vector;

	FILE *Matrix = fopen("matrix.dat","r");
	FILE *Vector = fopen("vector_input.dat","r");
	FILE *Output = fopen("output.txt","w");

	fseek(Vector,0,SEEK_END);
	int n = ftell(Vector);

	fseek(Matrix,0,SEEK_END);
	int mn = ftell(Matrix);

	int m = mn / n;

	//double matrixBuffer[mn];
	//double vectorBuffer[n];

	fseek(Vector,0,SEEK_SET);
	fseek(Matrix,0,SEEK_SET);

	fread(vectorBuffer, sizeof(double),n,Vector);
		
	for(i=0; i<m; i++){

		fread(&matrixBuffer,sizeof(double),n,Matrix);

		for(j=0; j<n; j++){
			
			mat_mul_pos = mat_index*4 + 2;
			vec_mul_pos = vec_index*2;
			
			double resultReal = matrixBuffer[mat_mul_pos].real * vectorBuffer[vec_mul_pos];
			double resultImg = matrixBuffer[mat_mul_pos+1].imaginary * vectorBuffer[vec_mul_pos+1];
			
			/*
			outputBuffer[j] = resultReal;
			outputBuffer[j+1] = resultImg;
			*/
			mat_index++;
			vec_index++;
		}

		for(int a=0; a<n; a++){
			outputBuffer[a]=multiplier(matrixBuffer[a],vectorBuffer[a]);
		}

		for(int b=0; b<n; b++){
			outputBuffer[0] = adder(outputBuffer[0],outputBuffer[b]);
		}

		fwrite(&outputBuffer[0].real,sizeof(double),1,Output);
		fwrite(&outputBuffer[0].imaginary,sizeof(double),1,Output);

		fseek(Matrix,n,SEEK_CUR);

	}

	fclose(Matrix);
	fclose(Vector);
	fclose(Output);
	
}

COMPLEX multiplier(COMPLEX num1, COMPLEX num2){
	COMPLEX mul;

	mul.real = num1.real*num2.real - num1.imaginary*num2.imaginary;
	mul.imaginary = num1.imaginary*num2.real + num1.real*num2.imaginary;

	return mul;
}

COMPLEX adder(COMPLEX num1, COMPLEX num2){
	COMPLEX sum;

	sum.real = num1.real + num2.real;
	sum.imaginary = num1.imaginary + num2.imaginary;

	return sum;
}