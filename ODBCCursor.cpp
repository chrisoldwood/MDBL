/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		ODBCCURSOR.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CODBCCursor class definition.
**
*******************************************************************************
*/

#include "MDBL.hpp"

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	hStmt	The statment handle.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CODBCCursor::CODBCCursor(const CODBCSource& oSource)
	: m_pSource(&oSource)
	, m_hStmt(SQL_NULL_HSTMT)
	, m_nColumns(0)
	, m_pColumns(NULL)
	, m_nRowLen(0)
	, m_nTotalLen(0)
	, m_pOffsets(NULL)
	, m_pRowData(NULL)
	, m_pRowStatus(NULL)
	, m_bDoneBind(false)
	, m_nFetched(0)
	, m_nCurRow((SQLUINTEGER)-1)
{

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

CODBCCursor::~CODBCCursor()
{
	Close();
}

/******************************************************************************
** Method:		Open()
**
** Description:	Opens the result set generated from the SQL statment handle.
**
** Parameters:	hStmt	The statment handle.
**
** Returns:		Nothing.
**
** Exceptions:	CSQLException on error.
**
*******************************************************************************
*/

void CODBCCursor::Open(SQLHSTMT hStmt)
{
	ASSERT(IsOpen() == false);
	ASSERT(hStmt    != SQL_NULL_HSTMT);

	// Save statement handle.
	m_hStmt = hStmt;

	// Get the number of columns in the result set.
	SQLRETURN rc = ::SQLNumResultCols(m_hStmt, &m_nColumns);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CSQLException(CSQLException::E_FETCH_FAILED, m_pSource->LastError(m_hStmt, SQL_HANDLE_STMT));

	ASSERT(m_nColumns > 0);

	// Allocate column list.
	m_pColumns = new SQLColumn[m_nColumns];

	// Retrieve column definitions.
	for (int i = 0; i < m_nColumns; i++)
	{
		SQLCHAR		szName[256] = "";
		SQLSMALLINT	nColumn = (SQLSMALLINT) (i+1);
		SQLSMALLINT	nNameLen = 256;
		SQLSMALLINT	nType;
		SQLUINTEGER	nSize;
		SQLSMALLINT	nScale;
		SQLSMALLINT	nNullable;

		// Get column definition.
		rc = ::SQLDescribeCol(m_hStmt, nColumn, szName, nNameLen,
								&nNameLen, &nType, &nSize, &nScale, &nNullable);

		if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
			throw CSQLException(CSQLException::E_FETCH_FAILED, m_pSource->LastError(m_hStmt, SQL_HANDLE_STMT));

		ASSERT(nType     != SQL_UNKNOWN_TYPE);
		ASSERT(nSize     != 0);
		ASSERT(nNullable != SQL_NULLABLE_UNKNOWN);

		// Convert definition to MDB types.
		m_pColumns[i].m_strName       = (const char*)szName;
		m_pColumns[i].m_nSQLColType   = nType;
		m_pColumns[i].m_eMDBColType   = CODBCSource::MDBType(nType);
		m_pColumns[i].m_nSQLFetchType = SQL_UNKNOWN_TYPE;
		m_pColumns[i].m_nSize         = nSize;
		m_pColumns[i].m_nFlags        = (nNullable == SQL_NULLABLE) ? CColumn::NULLABLE : CColumn::NOT_NULLABLE;

		ASSERT(m_pColumns[i].m_strName != "");
	}

	// Prepare for bind.
	m_bDoneBind = false;
}

/******************************************************************************
** Method:		Close()
**
** Description:	Closes the result set and frees up resources.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CODBCCursor::Close()
{
	// Free statement handle.
	if (m_hStmt != SQL_NULL_HSTMT)
		::SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);

	// Free internal buffers.
	delete[] m_pColumns;
	delete[] m_pOffsets;
	delete[] m_pRowData;
	delete[] m_pRowStatus;

	// Reset members.
	m_hStmt      = SQL_NULL_HSTMT;
	m_nColumns   = 0;
	m_pColumns   = NULL;
	m_nRowLen    = 0;
	m_nTotalLen  = 0;
	m_pOffsets   = NULL;
	m_pRowData   = NULL;
	m_pRowStatus = NULL;
	m_bDoneBind  = false;
	m_nFetched   = 0;
	m_nCurRow    = (SQLUINTEGER)-1;
}

/******************************************************************************
** Method:		IsOpen()
**
** Description:	Checks if the cursor is opened on a result set.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CODBCCursor::IsOpen() const
{
	return (m_hStmt != SQL_NULL_HSTMT);
}

/******************************************************************************
** Method:		NumColumns()
**
** Description:	Get the number of columns in the result set.
**
** Parameters:	None.
**
** Returns:		The number of columns.
**
*******************************************************************************
*/

int CODBCCursor::NumColumns() const
{
	return m_nColumns;
}

/******************************************************************************
** Method:		NumColumns()
**
** Description:	Get the number of columns in the result set.
**
** Parameters:	None.
**
** Returns:		The number of columns.
**
*******************************************************************************
*/

SQLColumn& CODBCCursor::Column(int n) const
{
	ASSERT( (n >= 0) && (n < m_nColumns) );

	return m_pColumns[n];
}

/******************************************************************************
** Method:		Bind()
**
** Description:	Bind the output buffers ready for fetching.
**
** Parameters:	None.
**
** Returns:		Nothing..
**
** Exceptions:	CSQLException on error.
**
*******************************************************************************
*/

void CODBCCursor::Bind()
{
	// Set column fetch types and sizes.
	for (int i = 0; i < m_nColumns; i++)
	{
		m_pColumns[i].m_nSQLFetchType = CODBCSource::ODBCType(m_pColumns[i].m_eMDBColType);
		m_pColumns[i].m_nSize         = CODBCSource::BufferSize(m_pColumns[i]);
	}

	m_nRowLen = 0;

	// Calculate the row buffer size.
	for (i = 0; i < m_nColumns; i++)
	{
		m_nRowLen += sizeof(SQLINTEGER);
		m_nRowLen += m_pColumns[i].m_nSize;
	}

	// Calculate the total buffer length.
	m_nTotalLen = m_nRowLen * FETCH_SIZE;

	// Allocate row buffers.
	m_pOffsets   = new int[m_nColumns];
	m_pRowData   = new byte[m_nTotalLen];
	m_pRowStatus = new SQLUSMALLINT[FETCH_SIZE];

	// Setup for bulk row fetching.
	::SQLSetStmtAttr(m_hStmt, SQL_ATTR_ROW_BIND_TYPE,    (SQLPOINTER)m_nRowLen,    0);
	::SQLSetStmtAttr(m_hStmt, SQL_ATTR_ROW_ARRAY_SIZE,   (SQLPOINTER)FETCH_SIZE,   0);
	::SQLSetStmtAttr(m_hStmt, SQL_ATTR_ROW_STATUS_PTR,   (SQLPOINTER)m_pRowStatus, 0);
	::SQLSetStmtAttr(m_hStmt, SQL_ATTR_ROWS_FETCHED_PTR, (SQLPOINTER)&m_nFetched,  0);

	int nOffset = 0;

	// Bind row buffers.
	for (i = 0; i < m_nColumns; i++)
	{
		// Save buffer offset to value.
		m_pOffsets[i] = nOffset;

		// Get bind parameters.
		SQLUSMALLINT nColumn = (SQLUSMALLINT) (i+1);
		SQLSMALLINT  nType   = (SQLSMALLINT) m_pColumns[i].m_nSQLFetchType;
		int          nSize   = m_pColumns[i].m_nSize;
		byte*		 pLenInd = m_pRowData + m_pOffsets[i];
		byte*        pValue  = pLenInd + sizeof(SQLINTEGER);

		ASSERT(nType != SQL_UNKNOWN_TYPE);
		ASSERT(nSize > 0);

		// Bind the buffer to the column.
		SQLRETURN rc = ::SQLBindCol(m_hStmt, nColumn, nType, pValue, nSize, (SQLINTEGER*) pLenInd);

		if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
			throw CSQLException(CSQLException::E_FETCH_FAILED, m_pSource->LastError(m_hStmt, SQL_HANDLE_STMT));

		// Update offset.
		nOffset += (sizeof(SQLINTEGER) + nSize);
	}

	// Prepare for fetch.
	m_nCurRow   = (SQLUINTEGER)-1;
	m_bDoneBind = true;
}

/******************************************************************************
** Method:		Fetch()
**
** Description:	Fetch the next row of data.
**
** Parameters:	None.
**
** Returns:		true or false.
**
** Exceptions:	CSQLException on error.
**
*******************************************************************************
*/

bool CODBCCursor::Fetch()
{
	ASSERT(IsOpen() == true);
	ASSERT(m_hStmt  != SQL_NULL_HSTMT);

	// Bound outputs yet?
	if (!m_bDoneBind)
		Bind();

	// Still retrieving current batch?
	if (++m_nCurRow < m_nFetched)
		return true;

	// Fetch the next bulk of rows.
	SQLRETURN rc = ::SQLFetch(m_hStmt);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) && (rc != SQL_NO_DATA) )
		throw CSQLException(CSQLException::E_FETCH_FAILED, m_pSource->LastError(m_hStmt, SQL_HANDLE_STMT));

	// Reset batch index.
	m_nCurRow = 0;

	// End Of Fetch?
	return (rc != SQL_NO_DATA);
}

/******************************************************************************
** Method:		SetRow()
**
** Description:	Copy the data from the internal buffers to the row.
**
** Parameters:	oRow	The row to update.
**
** Returns:		Nothing.
**
** Exceptions:	CSQLException on error.
**
*******************************************************************************
*/

void CODBCCursor::SetRow(CRow& oRow)
{
	SQLRETURN rc = m_pRowStatus[m_nCurRow];

	// Check row status.
	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CSQLException(CSQLException::E_FETCH_FAILED, m_pSource->LastError(m_hStmt, SQL_HANDLE_STMT));

	// Calculate pointer to the current row.
	byte* pRowData = m_pRowData + (m_nCurRow * m_nRowLen);

	// For all columns.
	for (int i = 0; i < m_nColumns; i++)
	{
		// Calculate pointer to value.
		byte* pValue = pRowData + m_pOffsets[i];

		// Get null/len indicator pointer.
		SQLINTEGER* pLenInd = (SQLINTEGER*) pValue;

		// Is value null?
		if (*pLenInd == SQL_NULL_DATA)
		{
			oRow[i] = null;
		}
		// Value is of native type?
		else if (m_pColumns[i].m_eMDBColType != MDCT_DATETIME)
		{
			oRow[i].SetRaw(pValue + sizeof(SQLINTEGER));
		}
		// Requires conversion.
		else
		{
			CTimeStamp* pTimeStamp = (CTimeStamp*)(pValue + sizeof(SQLINTEGER));
			time_t		tTime      = *pTimeStamp;

			oRow[i].SetRaw(&tTime);
		}
	}
}
