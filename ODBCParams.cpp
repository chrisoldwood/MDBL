/******************************************************************************
**
** MODULE:		ODBCPARAMS.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CODBCParams class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ODBCParams.hpp"
#include "ODBCSource.hpp"
#include "ODBCException.hpp"
#include "Row.hpp"
#include "TimeStamp.hpp"

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	oSource		The connection.
**				pszStmt		The statement that will be executed.
**				hStmt		The statement handle.
**				nParams		The number of parameters.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CODBCParams::CODBCParams(CODBCSource& oSource, const tchar* pszStmt, SQLHSTMT hStmt, size_t nParams)
	: m_oSource(oSource)
	, m_strStmt(pszStmt)
	, m_hStmt(hStmt)
	, m_nParams(static_cast<SQLSMALLINT>(nParams))
	, m_pParams(new SQLParam[nParams])
	, m_nRowLen(0)
	, m_pOffsets(NULL)
	, m_pRowData(NULL)
	, m_bDoneBind(false)
{
	ASSERT(hStmt   != SQL_NULL_HSTMT);
	ASSERT(nParams != 0);
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CODBCParams::~CODBCParams()
{
	// Free statement handle.
	if (m_hStmt != SQL_NULL_HSTMT)
		::SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);

	// Free internal buffers.
	delete[] m_pParams;
	delete[] m_pOffsets;
	delete[] m_pRowData;
}

/******************************************************************************
** Method:		NumParams()
**
** Description:	Get the number of parameters.
**
** Parameters:	None.
**
** Returns:		The number of parameters.
**
*******************************************************************************
*/

size_t CODBCParams::NumParams() const
{
	return m_nParams;
}

/******************************************************************************
** Method:		Param()
**
** Description:	Get a parameters details.
**
** Parameters:	n	The parameter.
**
** Returns:		The parameter details.
**
*******************************************************************************
*/

SQLParam& CODBCParams::Param(size_t n) const
{
	ASSERT(n < m_nParams);

	return m_pParams[n];
}

/******************************************************************************
** Method:		Bind()
**
** Description:	Creates and binds the input buffers to the statement.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CODBCParams::Bind()
{
	// Set parameter put types and sizes.
	for (size_t i = 0; i < m_nParams; ++i)
	{
		COLTYPE eType = m_pParams[i].m_eMDBColType;
		size_t  nSize = m_pParams[i].m_nMDBColSize;

		m_pParams[i].m_nBufType    = CODBCSource::ODBCType(eType);
		m_pParams[i].m_nBufSize    = CODBCSource::BufferSize(eType, nSize);
		m_pParams[i].m_nSQLColSize = CODBCSource::ColumnSize(eType, nSize);
	}

	m_nRowLen = 0;

	// Calculate the row buffer size.
	for (size_t i = 0; i < m_nParams; ++i)
	{
		m_nRowLen += sizeof(SQLINTEGER);
		m_nRowLen += m_pParams[i].m_nBufSize;
	}

	// Allocate row buffers.
	m_pOffsets = new size_t[m_nParams];
	m_pRowData = new byte[m_nRowLen];

	size_t nOffset = 0;

	// Bind all parameters.
	for (size_t i = 0; i < m_nParams; ++i)
	{
		// Save buffer offset to value.
		m_pOffsets[i] = nOffset;

		SQLUSMALLINT nParam   = static_cast<SQLUSMALLINT>(i+1);
		SQLSMALLINT  nBufType = static_cast<SQLSMALLINT>(m_pParams[i].m_nBufType);
		SQLSMALLINT  nSQLType = nBufType;
		size_t       nBufSize = m_pParams[i].m_nBufSize;
		size_t       nColSize = m_pParams[i].m_nSQLColSize;
		byte*	     pLenInd  = m_pRowData + m_pOffsets[i];
		byte*        pValue   = pLenInd + sizeof(SQLINTEGER);

		SQLRETURN rc = ::SQLBindParameter(m_hStmt, nParam, SQL_PARAM_INPUT, nBufType, nSQLType,
											nColSize, 0, pValue, nBufSize, reinterpret_cast<SQLLEN*>(pLenInd));

		if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
			throw CODBCException(CODBCException::E_ALLOC_FAILED, m_strStmt, m_hStmt, SQL_HANDLE_STMT);

		// Update offset.
		nOffset += (sizeof(SQLINTEGER) + nBufSize);
	}

	m_bDoneBind = true;
}

/******************************************************************************
** Method:		SetRow()
**
** Description:	Copies the row data into the buffers.
**
** Parameters:	oRow	The row to copy.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CODBCParams::SetRow(CRow& oRow)
{
	ASSERT(m_hStmt != SQL_NULL_HSTMT);

	// Bound inputs yet?
	if (!m_bDoneBind)
		Bind();

	// For all parameters.
	for (size_t iParam = 0; iParam < m_nParams; ++iParam)
	{
		size_t iRowCol = m_pParams[iParam].m_nSrcColumn;

		// Calculate pointer to value.
		byte* pValue = m_pRowData + m_pOffsets[iParam];

		// Get null/len indicator pointer.
		SQLLEN* pLenInd = reinterpret_cast<SQLLEN*>(pValue);

		// Is value null?
		if (oRow[iRowCol] == null)
		{
			*pLenInd = SQL_NULL_DATA;
		}
		// Requires conversion from MDCT_DATETIME?
		else if ( (m_pParams[iParam].m_eMDBColType == MDCT_DATETIME)
			   || (m_pParams[iParam].m_eMDBColType == MDCT_DATE)
			   || (m_pParams[iParam].m_eMDBColType == MDCT_TIME) )
		{
			CTimeStamp* pTimeStamp = reinterpret_cast<CTimeStamp*>(pValue + sizeof(SQLINTEGER));

			*pTimeStamp = oRow[iRowCol].GetDateTime();
			*pLenInd    = m_pParams[iParam].m_nBufSize;
		}
		// Requires conversion from MDCT_CHAR?
		else if (m_pParams[iParam].m_eMDBColType == MDCT_CHAR)
		{
			tchar* pszChar = reinterpret_cast<tchar*>(pValue + sizeof(SQLINTEGER));

			pszChar[0] = oRow[iRowCol];
			pszChar[1] = TXT('\0');
			*pLenInd   = SQL_NTS;
		}
		// Requires no conversion.
		else
		{
			oRow[iRowCol].GetRaw(pValue + sizeof(SQLINTEGER));

			if (m_pParams[iParam].m_nBufType != SQL_CHAR)
				*pLenInd = m_pParams[iParam].m_nBufSize;
			else
				*pLenInd = SQL_NTS;
		}
	}
}
