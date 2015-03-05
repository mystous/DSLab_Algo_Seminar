typedef struct POINTERS{
	FILE *matrix;
	FILE *vector;
	FILE *output;
}POINTERS;

typedef struct SIZE{
	double row;
	double col;
	double elem_num; // row * col
	long long file_size;
}SIZE;

typedef struct SIZES{
	SIZE matrix;
	SIZE vector;
	SIZE output;
}SIZES;

typedef struct BUF{
	double *matrix;
	double *vector;
	double *output;
}BUF;


typedef struct PT{	//mxn
	double m_start_idx;
	double m_end_idx;
	double n_start_idx;
	double n_end_idx;
}PT;
typedef struct P_IDX{
	PT* pt;
	double m_quotient;
	double n_quotient;
}P_IDX;
 
void open_file(POINTERS *fp);
void exit_process(POINTERS *fp);
void find_size(POINTERS *fp, SIZES* size);
void detect_error(POINTERS *fp, SIZES* size);
void read_data(POINTERS *fp, SIZES* size, BUF *buf);
void calc_data(POINTERS *fp, SIZES* size, BUF *buf);
void free_alloc(BUF *buf);
double real_calc(double A_real, double A_imag, double B_real, double B_imag);
double imag_calc(double A_real, double A_imag, double B_real, double B_imag);

void *thread_calc(void *);
void *thread_read(void *);
void make_thread(pthread_t **threads, int thr_num, void *(*func_p)(void* ));
void join_thread(pthread_t **threads, int thr_num, int *status);