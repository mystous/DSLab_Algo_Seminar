typedef struct MATRIX_SIZE{
	double m;
	double n;
}MATRIX_SIZE;

typedef struct MATRIX{
	double row;
	double column;
	double real;
	double imag;
	double prev_row;
	MATRIX_SIZE size;
}MATRIX;

typedef struct VECTOR{
	double real;
	double imag;
	double writing_real;
	double writing_imag;
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
int detect_error(MATRIX *matrix);

void print_matrix(MATRIX *matrix, FILE_POINTERS *fp);
void print_vector(VECTOR *vector, FILE_POINTERS *fp);
void print_result(VECTOR *result, FILE_POINTERS *fp);
int print_matrix_front(MATRIX *matrix, FILE_POINTERS *fp, int count);
int print_vector_front(VECTOR *vector, FILE_POINTERS *fp, int count);
int print_result_front(VECTOR *result, FILE_POINTERS *fp, int count);
void print_matrix_back(MATRIX *matrix, FILE_POINTERS *fp, int count);
void print_vector_back(VECTOR *vector, FILE_POINTERS *fp, int count);
void print_result_back(VECTOR *result, FILE_POINTERS *fp, int count);