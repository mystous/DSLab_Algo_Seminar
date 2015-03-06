// MVMul_MPI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// MVMul_MPI.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

#define BUFFER_SIZE		16000
#define MATRIX_FILE		"matrix.dat"
#define VECTOR_INPUT	"vector_input.dat"
#define VECTOR_OUTPUT	"vector_output.dat"

#define FREE_MEM(pointer)\
if (NULL != pointer)\
	{\
	free(pointer); \
	pointer = NULL; \
	}

typedef struct{
	double			fRowCount;
	double			fColumnCount;
	double			*pfRow;
	double			*pfColumn;
	double			*pfReal;
	double			*pfImaginary;
} MATRIX_VALUE, *LPMATRIX_VALUE;

typedef struct{
	double			fRowCount;
	double			*pfReal;
	double			*pfImaginary;
} VECTOR_VALUE, *LPVECTOR_VALUE;

MATRIX_VALUE	g_matrix;
VECTOR_VALUE	g_vector_input, g_vector_output;

long long g_nStart, g_nEnd, g_nSize;

void initVariables()
{
	g_matrix.fRowCount = 0;
	g_matrix.fColumnCount = 0;
	g_matrix.pfColumn = NULL;
	g_matrix.pfImaginary = NULL;
	g_matrix.pfReal = NULL;
	g_matrix.pfRow = NULL;

	g_vector_input.fRowCount = 0;
	g_vector_input.pfImaginary = NULL;
	g_vector_input.pfReal = NULL;

	g_vector_output.fRowCount = 0;
	g_vector_output.pfImaginary = NULL;
	g_vector_output.pfReal = NULL;
}

void FinalVariables()
{
	g_matrix.fRowCount = 0;
	g_matrix.fColumnCount = 0;
	FREE_MEM(g_matrix.pfColumn);
	FREE_MEM(g_matrix.pfImaginary);
	FREE_MEM(g_matrix.pfReal);
	FREE_MEM(g_matrix.pfRow);

	g_vector_input.fRowCount = 0;
	FREE_MEM(g_vector_input.pfImaginary);
	FREE_MEM(g_vector_input.pfReal);

	g_vector_output.fRowCount = 0;
	FREE_MEM(g_vector_output.pfImaginary);
	FREE_MEM(g_vector_output.pfReal);
}

void LoadBalance(int nTotalRank, int nMyRank, int nRow)
{
	g_nStart= nRow * nMyRank/nTotalRank+ 1;
    g_nEnd = nRow * (nMyRank+1)/nTotalRank;
    g_nSize = g_nEnd - g_nStart + 1;
}

bool LoadMatrixNVectorData(long long nRow, long long nColumn)
{
	bool		bRtn = false;
	FILE		*matrix = NULL, *vector = NULL;
	int			i;
	double		fRow, fColumn, fReal, fImagnary;
	double		fBuffer[BUFFER_SIZE];
	double		*fReadPtr = NULL;
	int			nReadCount;
	int			nReadIndex;
	long long	nElementIndex = 0;
	size_t		readSize;
	long long	nRankTotalSize = g_nSize * 4 * nColumn;
	long long	nPrevRankReadSize = (g_nStart -1) * sizeof(double) * 4 * nColumn, fReadCount = 0;
	long long	nReadTotal = 0;
	
	if (NULL != (matrix = fopen(MATRIX_FILE, "rb")))
	{
		g_matrix.fRowCount = nRow;
		g_matrix.fColumnCount = nColumn;
		g_matrix.pfReal = (double*)malloc(sizeof(double)*g_matrix.fRowCount*g_matrix.fColumnCount);
		g_matrix.pfImaginary = (double*)malloc(sizeof(double)*g_matrix.fRowCount*g_matrix.fColumnCount);
		g_matrix.pfRow = (double*)malloc(sizeof(double)*g_matrix.fRowCount*g_matrix.fColumnCount);
		g_matrix.pfColumn = (double*)malloc(sizeof(double)*g_matrix.fRowCount*g_matrix.fColumnCount);

		if (NULL == g_matrix.pfReal || NULL == g_matrix.pfImaginary || NULL == g_matrix.pfRow || NULL == g_matrix.pfReal)
			goto FREE_MEMORY;

		_fseeki64(matrix, nPrevRankReadSize, SEEK_SET);

		nElementIndex = 0;
		while (1)
		{
			readSize = fread(fBuffer, sizeof(double), BUFFER_SIZE, matrix);
			if (0 == readSize)
				break;

			if (0 != readSize % 4)
				goto FREE_MEMORY;

			nReadTotal += readSize;
			nReadIndex = 0;

			if( nReadTotal > nRankTotalSize )
				nReadCount = (nRankTotalSize - nReadTotal - readSize) / 4;
			else
				nReadCount = readSize / 4;

			for (i = 0; i < nReadCount ; ++i)
			{
				g_matrix.pfRow[nElementIndex] = fBuffer[nReadIndex++];
				g_matrix.pfColumn[nElementIndex] = fBuffer[nReadIndex++];
				g_matrix.pfReal[nElementIndex] = fBuffer[nReadIndex++];
				g_matrix.pfImaginary[nElementIndex++] = fBuffer[nReadIndex++];
			}
			
			if( nReadTotal > nRankTotalSize )
				break;
		}
		fclose(matrix);
	}
	
	if (NULL != (vector = fopen(VECTOR_INPUT, "rb")))
	{
		g_vector_input.fRowCount = nColumn;
		g_vector_input.pfReal = (double*)malloc(sizeof(double)*g_vector_input.fRowCount);
		g_vector_input.pfImaginary = (double*)malloc(sizeof(double)*g_vector_input.fRowCount);
		if (NULL == g_vector_input.pfReal || NULL == g_vector_input.pfImaginary)
			goto FREE_MEMORY;

		nElementIndex = 0;
		while (1)
		{
			readSize = fread(fBuffer, sizeof(double), BUFFER_SIZE, vector);
			if (0 == readSize)
				break;

			if (0 != readSize % 2)
				goto FREE_MEMORY;

			nReadIndex = 0;
			nReadCount = readSize / 2;
			for (i = 0; i < nReadCount; ++i)
			{
				g_vector_input.pfReal[nElementIndex] = fBuffer[nReadIndex++];
				g_vector_input.pfImaginary[nElementIndex++] = fBuffer[nReadIndex++];
			}
		}

		fclose(vector);
	}

	bRtn = true;
	return bRtn;

FREE_MEMORY:
	FREE_MEM(g_matrix.pfReal);
	FREE_MEM(g_matrix.pfImaginary);
	FREE_MEM(g_matrix.pfRow);
	FREE_MEM(g_matrix.pfReal);
	FREE_MEM(g_vector_input.pfReal);
	FREE_MEM(g_vector_input.pfImaginary);

	if( NULL != matrix )
		fclose(matrix);
	if( NULL != vector)
		fclose(vector);
	bRtn = false;
	return bRtn;
}

bool MVMul()
{
	bool bRtn = false;
	double	i, j;

	if (0 == g_matrix.fRowCount || 0 == g_matrix.fColumnCount || 0 == g_vector_input.fRowCount || g_vector_input.fRowCount != g_matrix.fColumnCount)
		return bRtn;

	g_vector_output.fRowCount = g_matrix.fRowCount;
	g_vector_output.pfReal = (double*)malloc(sizeof(double)*g_vector_output.fRowCount);
	g_vector_output.pfImaginary = (double*)malloc(sizeof(double)*g_vector_output.fRowCount);

	if (NULL == g_vector_output.pfReal || NULL == g_vector_output.pfImaginary)
	{
		FREE_MEM(g_vector_output.pfReal);
		FREE_MEM(g_vector_output.pfImaginary);
		return bRtn;
	}

	for (i = 0; i < g_matrix.fRowCount; ++i)
	{
		g_vector_output.pfReal[(int)i] = 0.;
		g_vector_output.pfImaginary[(int)i] = 0.;
		for (j = 0; j < g_matrix.fColumnCount; ++j)
		{
			double		fMatrixReal = g_matrix.pfReal[(int)i*(int)g_matrix.fColumnCount + (int)j];
			double		fMatrixImaginary = g_matrix.pfImaginary[(int)i*(int)g_matrix.fColumnCount + (int)j];
			g_vector_output.pfReal[(int)i] += fMatrixReal * g_vector_input.pfReal[(int)j] - fMatrixImaginary * g_vector_input.pfImaginary[(int)j];
			g_vector_output.pfImaginary[(int)i] += fMatrixReal * g_vector_input.pfImaginary[(int)j] + fMatrixImaginary * g_vector_input.pfReal[(int)j];
		}
	}

	bRtn = true;
	return bRtn;
}

bool SaveResult(int nMyRank, int nTotalRank)
{
	FILE				*dataFile;
	bool				bRtn = false;
	double				i;

	MPI_Request			req;
	double				fTemp = 1;

	if (0 == nMyRank)
		dataFile = fopen(VECTOR_OUTPUT, "wb");
	else
	{
		MPI_Status status;
		MPI_Recv(&fTemp, 1, MPI_DOUBLE, nMyRank-1, nMyRank, MPI_COMM_WORLD, &status);
		dataFile = fopen(VECTOR_OUTPUT, "ab");
	}

	if (NULL != dataFile )
	{
		for (i = 0; i < g_vector_output.fRowCount; ++i)
		{
			fwrite(&g_vector_output.pfReal[(int)i], sizeof(double), 1, dataFile);
			fwrite(&g_vector_output.pfImaginary[(int)i], sizeof(double), 1, dataFile);
		}
		bRtn = true;
	}

	fclose(dataFile);
	if (nMyRank < nTotalRank - 1)
		MPI_Send(&fTemp, 1, MPI_DOUBLE, nMyRank + 1, nMyRank + 1, MPI_COMM_WORLD);

	return bRtn;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int								nRank, nTotalRank;
	double							fStartTime, fEndTime;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &nTotalRank);
	MPI_Comm_rank(MPI_COMM_WORLD, &nRank);

	initVariables();
	LoadBalance(nTotalRank, nRank, atoi(argv[1]));
	
	if( 0 == nRank )
		fStartTime = MPI_Wtime();

	if(LoadMatrixNVectorData(g_nSize, atoi(argv[2])))
	{
		if( 0 == nRank )
		{
			fEndTime = MPI_Wtime();
			printf("File Read Time: %lf\n", fEndTime - fStartTime);
		}

		MVMul();
		
		if( 0 == nRank )
		{
			fStartTime = MPI_Wtime();
			printf("MVMul Time: %lf\n", fStartTime - fEndTime);
		}

		SaveResult(nRank, nTotalRank);

		if( 0 == nRank )
		{
			fEndTime = MPI_Wtime();
			printf("File Writing Time: %lf\n", fEndTime - fStartTime);
		}
	}
	else
	{
		if( 0 == nRank )
			printf("Can't allocate memeory!\n");
	}

	FinalVariables();

	MPI_Finalize();
	return 0;
}
