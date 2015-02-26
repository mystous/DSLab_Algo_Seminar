typedef struct MATRIX_SIZE{
	double n;
	double m;
}MATRIX_SIZE;

typedef struct MATRIX{
	double row;
	double column;
	double real;
	double imag;
	MATRIX_SIZE size;
}MATRIX;

typedef struct VECTOR{
	double real_temp;
	double imag_temp;
	//double real;
	//double imag;
}VECTOR;

typedef struct FILE_POINTERS{
	FILE *matrix;
	FILE *vector;
	FILE *result;
}FILE_POINTERS;

void read_matrix(MATRIX *matrix, FILE_POINTERS *fp);
void read_vector(VECTOR *vector, FILE_POINTERS *fp);
void read_result(VECTOR *result, FILE_POINTERS *fp);
void write_result(VECTOR *result, FILE_POINTERS *fp);

void find_matrix_size(MATRIX *matrix, FILE_POINTERS *fp);

void calc(MATRIX *matrix, VECTOR *vector, VECTOR *result);

void process_exit(FILE_POINTERS *fp);

void print_matrix(MATRIX *matrix, FILE_POINTERS *fp);
void print_vector(VECTOR *vector, FILE_POINTERS *fp);
void print_result(VECTOR *result, FILE_POINTERS *fp);
