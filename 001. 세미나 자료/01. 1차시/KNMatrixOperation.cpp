#include "stdafx.h"
#include "KNMatrixOperation.h"

#include "CKNGlobal.h"




///------------------------------------------------------------------------------------------
///
///			CKNVector Class member function - Start
///
///------------------------------------------------------------------------------------------
CKNMatrixOperation::CKNVector::CKNVector()
{
}

CKNMatrixOperation::CKNVector::~CKNVector()
{

}

/**
	@param nSize: Vector elements size
*/
void CKNMatrixOperation::CKNVector::SetSize(unsigned int nSize)
{
	if (NULL != m_pfValueBuffer)
	{
		free(m_pfValueBuffer);
		m_pfValueBuffer = NULL;
	}

	m_nValueCount = nSize;
	m_pfValueBuffer = new CKNComplex[m_nValueCount];
	if (NULL == m_pfValueBuffer)
	{
		THROW_END_EXIT(ERROR_MALLOC);
	}

	//memset(m_pfValueBuffer, 0, sizeof(double)*m_nValueCount);
}

/**
	@param nIndex: Specific element index that want to set
	@param value: element value
*/
void CKNMatrixOperation::CKNVector::SetAt(unsigned int nIndex, CKNComplex value)
{
	SetAt(nIndex, value.GetRealNumber(), value.GetImaginaryNumber());
}

/**
	@param nIndex: Specific element index that want to set
	@param fReal: Real number of element value
	@param fImaginary: Imaginary number of element value
*/
void CKNMatrixOperation::CKNVector::SetAt(unsigned int nIndex, double fReal, double fImaginary)
{
	if (nIndex > GetSize())
	{
		THROW_END_EXIT(ERROR_OUT_OF_RANGE);
	}

	m_pfValueBuffer[nIndex].SetComplexNumber(fReal, fImaginary);
}

/**
	@param nIndex: Specific element index that want to get
	@return element value
*/
CKNComplex CKNMatrixOperation::CKNVector::GetAt(unsigned int nIndex)
{
	CKNComplex		nullComplex;

	if (nIndex > GetSize())
	{
		throw ERROR_OUT_OF_RANGE;
		return nullComplex;
	}

	return m_pfValueBuffer[nIndex];
}

/**
	@param fScalar: Scalar value that want to use in operation
*/
void CKNMatrixOperation::CKNVector::ScalarMultiple(double fScalar)
{
	unsigned int			i;

	for (i = 0; i < GetSize(); i++)
	{
		m_pfValueBuffer[i] *= fScalar;
	}
}

/**
@param fScalar: Scalar value that want to use in operation
*/
void CKNMatrixOperation::CKNVector::ScalarDivision(double fScalar)
{
	unsigned int			i;

	for (i = 0; i < GetSize(); i++)
	{
		m_pfValueBuffer[i] /= fScalar;
	}
}

void CKNMatrixOperation::CKNVector::BuildRandomVector()
{
	unsigned int		i;

	srand((unsigned int)time(NULL));
	for (i = 0; i < m_nValueCount; i++)
		m_pfValueBuffer[i].SetComplexNumber(rand(), rand());
}

void CKNMatrixOperation::CKNVector::Normalize()
{
	double	fVectorSize = GetNorm();
	unsigned int		i;

	for (i = 0; i < m_nValueCount; i++)
		m_pfValueBuffer[i] /= fVectorSize;
}

double CKNMatrixOperation::CKNVector::GetNorm()
{
	unsigned int	i;
	CKNComplex		PowerTotal;

	for (i = 0; i < m_nValueCount; i++)
		PowerTotal += (m_pfValueBuffer[i] * m_pfValueBuffer[i]);

	return PowerTotal.GetNorm();
}

/**
	@param vector: vector that is used in minus operation
*/
void CKNMatrixOperation::CKNVector::MinusVector(CKNVector *vector)
{
	unsigned int	i, nSize = GetSize();

	if (nSize != vector->GetSize())
	{
		throw ERROR_WRONG_ORDER_OPERATION;
		return;
	}

	SetSize(nSize);
	for (i = 0; i < nSize; i++)
	{
		SetAt(i, GetAt(i) - vector->GetAt(i));
	}
}

void CKNMatrixOperation::CKNVector::Finalize()
{
	if (NULL != m_pfValueBuffer)
	{
		free(m_pfValueBuffer);
		m_pfValueBuffer = NULL;
	}

	m_nValueCount = 0;
}

CKNComplex CKNMatrixOperation::CKNVector::operator*(CKNVector * vector)
{
	return operator*((*vector));
}

CKNComplex CKNMatrixOperation::CKNVector::operator*(CKNVector & vector)
{
	CKNComplex Rtn;
	unsigned int		i, nSize = GetSize();

	if (nSize != vector.GetSize())
	{
		throw ERROR_WRONG_ORDER_OPERATION;
		return Rtn;
	}

	for (i = 0; i < nSize; i++)
	{
		Rtn += (GetAt(i) * vector.GetAt(i));
	}

	return Rtn;
}

CKNMatrixOperation::CKNVector CKNMatrixOperation::CKNVector::operator-(CKNVector * vector)
{
	return operator-((*vector));
}

CKNMatrixOperation::CKNVector CKNMatrixOperation::CKNVector::operator-(CKNVector & vector)
{
	CKNVector		rtnVector;
	unsigned int	i, nSize = GetSize();

	if (nSize != vector.GetSize())
	{
		throw ERROR_WRONG_ORDER_OPERATION;
		return rtnVector;
	}

	rtnVector.SetSize(nSize);
	for (i = 0; i < nSize; i++)
	{
		rtnVector.SetAt(i, GetAt(i) - vector.GetAt(i));
	}

	return rtnVector;
}

CKNMatrixOperation::CKNVector CKNMatrixOperation::CKNVector::operator+(CKNVector * vector)
{
	return operator+((*vector));
}

CKNMatrixOperation::CKNVector CKNMatrixOperation::CKNVector::operator+(CKNVector & vector)
{
	CKNVector		rtnVector;
	unsigned int	i, nSize = GetSize();

	if (nSize != vector.GetSize())
	{
		throw ERROR_WRONG_ORDER_OPERATION;
		return rtnVector;
	}

	rtnVector.SetSize(nSize);
	for (i = 0; i < nSize; i++)
	{
		rtnVector.SetAt(i, GetAt(i) + vector.GetAt(i));
	}

	return rtnVector;
}

void CKNMatrixOperation::CKNVector::operator=(CKNVector *vector)
{
	operator=((*vector));
}

void CKNMatrixOperation::CKNVector::operator=(CKNVector &vector)
{
	unsigned int		i, nSize = vector.GetSize();
	
	SetSize(nSize);
	for (i = 0; i < nSize; i++)
	{
		SetAt(i, vector.GetAt(i));
	}
}
///------------------------------------------------------------------------------------------
///
///			CKNVector Class member function - End
///
///------------------------------------------------------------------------------------------


///------------------------------------------------------------------------------------------
///
///			CKNCSR Class member function - Start
///
///------------------------------------------------------------------------------------------
CKNMatrixOperation::CKNCSR::CKNCSR()
{
}

CKNMatrixOperation::CKNCSR::~CKNCSR()
{
	
}

void CKNMatrixOperation::CKNCSR::IncreaseNoneZeroCount()
{
	m_nValueCount++;
}

void CKNMatrixOperation::CKNCSR::BuildDataBuffer()
{
	/// TODO: 예외처리 할 수 있는 오류를 Return하는 것으로 변경해야 함
	if (0 == m_nValueCount)
		return;

	m_pValueBuffer = new CKNComplex[m_nValueCount];

	if (NULL == m_pValueBuffer)
	{
		THROW_END_EXIT(ERROR_MALLOC);
	}

	m_pnColum = (unsigned int*)malloc(sizeof(unsigned int)*m_nValueCount);
	if (NULL == m_pnColum)
	{
		Finalize();
		THROW_END_EXIT(ERROR_MALLOC);
	}

	m_pnRow = (unsigned int*)malloc(sizeof(unsigned int)*(m_nRowCount + 1));
	if (NULL == m_pnRow)
	{
		Finalize();
		THROW_END_EXIT(ERROR_MALLOC);
	}
	else
	{
		/// Set to buffer with Max value OS has whatever bit
		memset(m_pnRow, -1, sizeof(unsigned int)*(m_nRowCount + 1));
	}
}

/**
	@param nIndex: Index that want to get row information
	@return row information that saved
*/
unsigned int CKNMatrixOperation::CKNCSR::GetRowIndexNo(unsigned int nIndex)
{
	if (nIndex > GetRowCount() + 1)
	{
		throw ERROR_OUT_OF_RANGE;
		return MAX_INDEX;
	}

	return m_pnRow[nIndex];
}

/**
@param nIndex: Index that want to get column information
@return column information that saved
*/
unsigned int CKNMatrixOperation::CKNCSR::GetColIndexNo(unsigned int nIndex)
{
	if (nIndex > GetNoneZeroCount())
	{
		throw ERROR_OUT_OF_RANGE;
		return MAX_INDEX;
	}

	return m_pnColum[nIndex];
}

/**
	@param nIndex: Index that want to get none zero value
	@return Complex number
*/
CKNComplex CKNMatrixOperation::CKNCSR::GetValue(unsigned int nIndex)
{
	CKNComplex		dumyValue;
	if (nIndex > GetNoneZeroCount())
	{
		throw ERROR_OUT_OF_RANGE;
		return dumyValue;
	}

	return m_pValueBuffer[nIndex];
}

/**
	@param fRealValue: None zero value - Real part
	@param fImaginaryValue: None zero value - Imaginary part
	@param nRow: Row index of none zero value
	@param nCol: Column index of none zero value
*/
void CKNMatrixOperation::CKNCSR::PushNoneZeroValue(double fRealValue, double fImaginaryValue, unsigned int nRow, unsigned int nCol)
{
	m_pValueBuffer[m_nValueStackCount].SetComplexNumber(fRealValue, fImaginaryValue);
	m_pnColum[m_nValueStackCount] = nCol;
	if (MAX_INDEX == m_pnRow[nRow])
		m_pnRow[nRow] = m_nValueStackCount;
	
	m_nValueStackCount++;
}

void CKNMatrixOperation::CKNCSR::FinishPush()
{
	m_pnRow[GetRowCount()] = GetNoneZeroCount();
}

void CKNMatrixOperation::CKNCSR::Finalize()
{
	if (NULL == this)
		return;

	if (NULL != this->m_pnColum)
	{
		free(this->m_pnColum);
		this->m_pnColum = NULL;
	}

	if (NULL != this->m_pValueBuffer)
	{
		delete [] this->m_pValueBuffer;
		this->m_pValueBuffer = NULL;
	}

	if (NULL != this->m_pnRow)
	{
		free(this->m_pnRow);
		this->m_pnRow = NULL;
	}
}

CKNMatrixOperation::CKNVector CKNMatrixOperation::CKNCSR::operator*(CKNVector  & vector)
{
	CKNMatrixOperation::CKNVector	rtnVector;
	unsigned int					i, j, nSize = GetRowCount();

	if (nSize != vector.GetSize())
	{
		throw ERROR_WRONG_ORDER_OPERATION;
		return rtnVector;
	}

	rtnVector.SetSize(GetRowCount());
	for (i = 0; i < nSize; i++)
	{
		CKNComplex			fSubTotal = 0;
		unsigned int	nSubStart = GetRowIndexNo(i), nSubEnd = GetRowIndexNo(i+1);

		for (j = nSubStart; j < nSubEnd; j++)
		{
			unsigned int nColIndex = GetColIndexNo(j);
			fSubTotal += (GetValue(j)*vector.GetAt(nColIndex));
		}
		rtnVector.SetAt(i, fSubTotal);
	}

	return rtnVector;
}

CKNMatrixOperation::CKNVector CKNMatrixOperation::CKNCSR::operator*(CKNVector  * vector)
{
	return operator*((*vector));
}
///------------------------------------------------------------------------------------------
///
///			CKNCSR Class member function - End
///
///------------------------------------------------------------------------------------------


///------------------------------------------------------------------------------------------
///
///			CKNMatrixOperation Class member function - End
///
///------------------------------------------------------------------------------------------
CKNMatrixOperation::CKNMatrixOperation()
{
}


CKNMatrixOperation::~CKNMatrixOperation()
{
}

/**
	@param pReal: Real number part of Source of matrix
	@param pImaginary: Imaginary number part of Source of matrix
	@param nOrder: The order of matrix
*/
CKNMatrixOperation::CKNCSR * CKNMatrixOperation::BuildCSRFromOneDimArray(double *pReal, double *pImaginary, unsigned int nOrder)
{
	CKNMatrixOperation::CKNCSR *pCSR = new CKNMatrixOperation::CKNCSR();

	if (NULL == pCSR)
		return pCSR;

	pCSR->SetRowCount(nOrder);

	/// Commented by Kyu Nam Cho - 우선 None zero count를 모두 세고 시작한다.
	/// 향후 Count loop와 CSR Build loop를 하나로 합친다.
	/// Matrix의 크기를 무한대로 잡아야 하는데 for loop를 무한으로 돌것인가? 고민
	unsigned int row, col;
	for (row = 0; row < nOrder; row++)
	{
		for (col = 0; col < nOrder; col++)
		{
			if (0 != pReal[row*nOrder + col] || 0 != pImaginary[row*nOrder + col])
				pCSR->IncreaseNoneZeroCount();
		}
	}
	pCSR->BuildDataBuffer();

	unsigned int nIndex = 0;
	bool bPushedInRow = false;
	for (row = 0; row < nOrder; row++)
	{
		for (col = 0; col < nOrder; col++)
		{
			if (0 != pReal[row*nOrder + col] || 0 != pImaginary[row*nOrder + col])
			{
				pCSR->PushNoneZeroValue(pReal[nIndex], pImaginary[nIndex], row, col);
				bPushedInRow = true;
			}
			nIndex++;
		}
	}
	pCSR->FinishPush();

	return pCSR;
}

/**
	@param pCSR: CSR class pointer that want to deallocating memeory
*/
void CKNMatrixOperation::FreeCSR(CKNMatrixOperation::CKNCSR *pCSR)
{
	if (NULL == pCSR)
		return;

	pCSR->Finalize();

	delete pCSR;
	pCSR = NULL;
}

/**
	@param pstrFileName: File name for saving
*/
void CKNMatrixOperation::CKNCSR::DumpCSR(const char *pstrFileName)
{
	FILE		*out;
	unsigned int			i, nCount;

	out = fopen(pstrFileName, "wt");
	if (NULL != out)
	{
		fputs("None Zero Value\r\n", out);
		fputs("------------------------------------------\r\n", out);
		nCount = GetNoneZeroCount();
		for (i = 0; i < nCount; i++)
			fprintf(out, "%f + %fi\r\n", m_pValueBuffer[i].GetRealNumber(), m_pValueBuffer[i].GetImaginaryNumber());

		fputs("Column\r\n", out);
		fputs("------------------------------------------\r\n", out);
		nCount = GetNoneZeroCount();
		for (i = 0; i < nCount; i++)
			fprintf(out, "%u\r\n", m_pnColum[i]);

		fputs("Row\r\n", out);
		fputs("------------------------------------------\r\n", out);
		nCount = GetRowCount() + 1;
		for (i = 0; i < nCount; i++)
			fprintf(out, "%u\r\n", m_pnRow[i]);

		fclose(out);
	}
}

/**
	@param pAMatrix: Matrix class pointer that want to operation
	@param pVector: Vector class pointer that want to operation
	@param pResult: Vector class pointer that saving operation result
*/
void CKNMatrixOperation::MVMul(CKNCSR *pAMatrix, CKNVector *pVector, CKNVector *pResult)
{
	unsigned int					i, j, nSize = pAMatrix->GetRowCount();

	if (nSize != pVector->GetSize())
	{
		throw ERROR_WRONG_ORDER_OPERATION;
		return;
	}

	pResult->SetSize(pAMatrix->GetRowCount());
	for (i = 0; i < nSize; i++)
	{
		CKNComplex		fSubTotal;
		unsigned int	nSubStart = pAMatrix->GetRowIndexNo(i), nSubEnd = pAMatrix->GetRowIndexNo(i + 1);

		for (j = nSubStart; j < nSubEnd; j++)
		{
			unsigned int nColIndex = pAMatrix->GetColIndexNo(j);
			fSubTotal += (pAMatrix->GetValue(j)*pVector->GetAt(nColIndex));
		}
		pResult->SetAt(i, fSubTotal);
	}
}

/**
	@param pVector1: Vector class pointer that want to operation
	@param pVector2: Vector class pointer that want to operation
	@return A result of operation
*/
double CKNMatrixOperation::VVDot(CKNVector *pVector1, CKNVector *pVector2)
{
	double fRtn = 0.;
	unsigned int		i, nSize = pVector1->GetSize();

	if (nSize != pVector2->GetSize())
	{
		throw ERROR_WRONG_ORDER_OPERATION;
		return 0;
	}

	for (i = 0; i < nSize; i++)
	{
		fRtn += (pVector1->GetAt(i) * pVector2->GetAt(i));
	}

	return fRtn;
}

CKNMatrixOperation::CKNVector operator*(const double fScalar, CKNMatrixOperation::CKNVector & vector)
{
	vector.ScalarMultiple(fScalar);
	return vector;
}

CKNMatrixOperation::CKNVector operator/(CKNMatrixOperation::CKNVector & vector, const double fScalar)
{
	vector.ScalarDivision(fScalar);
	return vector;
}
///------------------------------------------------------------------------------------------
///
///			CKNMatrixOperation Class member function - End
///
///------------------------------------------------------------------------------------------