/******************************************************************************
**
** MODULE:		ODBCCURSOR.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CODBCCursor class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ODBCCursor.hpp"
#include "ODBCException.hpp"
#include "ODBCSource.hpp"
#include "Column.hpp"
#include "Row.hpp"
#include "TimeStamp.hpp"

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

CODBCCursor::CODBCCursor(CODBCSource& oSource)
	: m_oSource(oSource)
	, m_strStmt()
	, m_hStmt(SQL_NULL_HSTMT)
	, m_nColumns(0)
	, m_pColumns(nullptr)
	, m_nRowLen(0)
	, m_nTotalLen(0)
	, m_pOffsets(nullptr)
	, m_pRowData(nullptr)
	, m_pRowStatus(nullptr)
	, m_bDoneBind(false)
	, m_nFetched(0)
	, m_nCurRow(static_cast<SQLUINTEGER>(-1))
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
** Parameters:	pszStmt		The statment executed.
**				hStmt		The statment handle.
**
** Returns:		Nothing.
**
** Exceptions:	CODBCException on error.
**
*******************************************************************************
*/

void CODBCCursor::Open(const tchar* pszStmt, SQLHSTMT hStmt)
{
	ASSERT(IsOpen() == false);
	ASSERT(hStmt    != SQL_NULL_HSTMT);

	// Save statement and handle.
	m_strStmt = pszStmt;
	m_hStmt   = hStmt;

	// Find the result set.
	// NB: Can't assume first result set if a stored procedure has been called.
	while (m_nColumns == 0)
	{
		SQLSMALLINT nColumns = 0;

		// Get the number of columns in the result set.
		SQLRETURN rc = ::SQLNumResultCols(m_hStmt, &nColumns);

		if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
			throw CODBCException(CODBCException::E_FETCH_FAILED, m_strStmt, m_hStmt, SQL_HANDLE_STMT);

		m_nColumns = nColumns;

		// Result set contains rows?
		if (m_nColumns != 0)
			break;

		// Move to next result set.
		rc = ::SQLMoreResults(m_hStmt);

		if (rc == SQL_NO_DATA)
			break;

		if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
			throw CODBCException(CODBCException::E_FETCH_FAILED, m_strStmt, m_hStmt, SQL_HANDLE_STMT);
	}

	// Allocate column list.
	m_pColumns = new ODBCColumn[m_nColumns];

	// Retrieve column definitions.
	for (size_t i = 0; i < m_nColumns; ++i)
	{
		const size_t MAX_NAME_LEN = 256;

		SQLTCHAR	szName[MAX_NAME_LEN+1] = { 0 };
		SQLSMALLINT	nColumn = static_cast<SQLSMALLINT>(i+1);
		SQLSMALLINT	nNameLen = MAX_NAME_LEN;
		SQLSMALLINT	nType;
		SQLULEN		nSize;
		SQLSMALLINT	nScale;
		SQLSMALLINT	nNullable;

		// Get column definition.
		SQLRETURN rc = ::SQLDescribeCol(m_hStmt, nColumn, szName, nNameLen,
										&nNameLen, &nType, &nSize, &nScale, &nNullable);

		if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
			throw CODBCException(CODBCException::E_FETCH_FAILED, m_strStmt, m_hStmt, SQL_HANDLE_STMT);

		ASSERT(nType     != SQL_UNKNOWN_TYPE);
		ASSERT(nSize     != 0);
		ASSERT(nNullable != SQL_NULLABLE_UNKNOWN);

		// Convert definition to MDB types.
		m_pColumns[i].m_nDstColumn    = i;
		m_pColumns[i].m_strName       = reinterpret_cast<tchar*>(szName);
		m_pColumns[i].m_nSQLColType   = nType;
		m_pColumns[i].m_eMDBColType   = CODBCSource::MDBType(nType);
		m_pColumns[i].m_nSQLFetchType = SQL_UNKNOWN_TYPE;
		m_pColumns[i].m_nSize         = nSize;
		m_pColumns[i].m_nFlags        = (nNullable == SQL_NULLABLE) ? CColumn::NULLABLE : CColumn::NOT_NULLABLE;
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
	m_pColumns   = nullptr;
	m_nRowLen    = 0;
	m_nTotalLen  = 0;
	m_pOffsets   = nullptr;
	m_pRowData   = nullptr;
	m_pRowStatus = nullptr;
	m_bDoneBind  = false;
	m_nFetched   = 0;
	m_nCurRow    = static_cast<SQLUINTEGER>(-1);
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

size_t CODBCCursor::NumColumns() const
{
	return m_nColumns;
}

/******************************************************************************
** Method:		Column()
**
** Description:	Get a columns details.
**
** Parameters:	n	The column.
**
** Returns:		The column details.
**
*******************************************************************************
*/

const SQLColumn& CODBCCursor::Column(size_t n) const
{
	ASSERT(n < m_nColumns);

	return m_pColumns[n];
}

////////////////////////////////////////////////////////////////////////////////
//! Set the mapping from result set column to table column.

void CODBCCursor::MapColumn(size_t sourceColumn, size_t destColumn, COLTYPE type, size_t size)
{
	ASSERT(sourceColumn < m_nColumns);

	SQLColumn& column = m_pColumns[sourceColumn];

	column.m_nDstColumn  = destColumn;
	column.m_eMDBColType = type;
	column.m_nSize       = size;
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
** Exceptions:	CODBCException on error.
**
*******************************************************************************
*/

void CODBCCursor::Bind()
{
	SQLRETURN rc;

	// Set column fetch types and sizes.
	for (size_t i = 0; i < m_nColumns; ++i)
	{
		m_pColumns[i].m_nSQLFetchType = CODBCSource::ODBCType(m_pColumns[i].m_eMDBColType);
		m_pColumns[i].m_nSize         = CODBCSource::BufferSize(m_pColumns[i].m_eMDBColType, m_pColumns[i].m_nSize);
	}

	m_nRowLen = 0;

	// Calculate the row buffer size.
	for (size_t i = 0; i < m_nColumns; ++i)
	{
		m_nRowLen += sizeof(SQLLEN);
		m_nRowLen += m_pColumns[i].m_nSize;
	}

	// Calculate the total buffer length.
	m_nTotalLen = m_nRowLen * FETCH_SIZE;

	// Allocate row buffers.
	m_pOffsets   = new size_t[m_nColumns];
	m_pRowData   = new byte[m_nTotalLen];
	m_pRowStatus = new SQLUSMALLINT[FETCH_SIZE];

	// Setup for bulk row fetching.
	rc = ::SQLSetStmtAttr(m_hStmt, SQL_ATTR_ROW_BIND_TYPE,    reinterpret_cast<SQLPOINTER>(m_nRowLen),    0);
	rc = ::SQLSetStmtAttr(m_hStmt, SQL_ATTR_ROW_ARRAY_SIZE,   reinterpret_cast<SQLPOINTER>(FETCH_SIZE),   0);
	rc = ::SQLSetStmtAttr(m_hStmt, SQL_ATTR_ROW_STATUS_PTR,   reinterpret_cast<SQLPOINTER>(m_pRowStatus), 0);
	rc = ::SQLSetStmtAttr(m_hStmt, SQL_ATTR_ROWS_FETCHED_PTR, reinterpret_cast<SQLPOINTER>(&m_nFetched),  0);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_FETCH_FAILED, m_strStmt, m_hStmt, SQL_HANDLE_STMT);

	size_t nOffset = 0;

	// Bind row buffers.
	for (size_t i = 0; i < m_nColumns; ++i)
	{
		// Save buffer offset to value.
		m_pOffsets[i] = nOffset;

		// Get bind parameters.
		SQLUSMALLINT nColumn = static_cast<SQLUSMALLINT>(i+1);
		SQLSMALLINT  nType   = static_cast<SQLSMALLINT>(m_pColumns[i].m_nSQLFetchType);
		size_t       nSize   = m_pColumns[i].m_nSize;
		byte*		 pLenInd = m_pRowData + m_pOffsets[i];
		byte*        pValue  = pLenInd + sizeof(SQLLEN);

		ASSERT(nType != SQL_UNKNOWN_TYPE);
		ASSERT(nSize > 0);

		// Bind the buffer to the column.
		rc = ::SQLBindCol(m_hStmt, nColumn, nType, pValue, nSize, reinterpret_cast<SQLLEN*>(pLenInd));

		if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
			throw CODBCException(CODBCException::E_FETCH_FAILED, m_strStmt, m_hStmt, SQL_HANDLE_STMT);

		// Update offset.
		nOffset += (sizeof(SQLLEN) + nSize);
	}

	// Prepare for fetch.
	m_nCurRow   = static_cast<SQLUINTEGER>(-1);
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
** Exceptions:	CODBCException on error.
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
		throw CODBCException(CODBCException::E_FETCH_FAILED, m_strStmt, m_hStmt, SQL_HANDLE_STMT);

	// Reset batch index.
	m_nCurRow = 0;

	// End Of Fetch?
	return (rc != SQL_NO_DATA);
}

/******************************************************************************
** Method:		GetRow()
**
** Description:	Copy the data from the internal buffers to the row.
**
** Parameters:	oRow	The row to update.
**
** Returns:		Nothing.
**
** Exceptions:	CODBCException on error.
**
*******************************************************************************
*/

void CODBCCursor::GetRow(CRow& oRow)
{
	SQLRETURN rc = m_pRowStatus[m_nCurRow];

	// Check row status.
	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_FETCH_FAILED, m_strStmt, m_hStmt, SQL_HANDLE_STMT);

	// Calculate pointer to the current row.
	byte* pRowData = m_pRowData + (m_nCurRow * m_nRowLen);

	// For all SQL columns.
	for (size_t iSQLCol = 0; iSQLCol < m_nColumns; ++iSQLCol)
	{
#ifdef _DEBUG
		// There is currently no ODBC -> COLTYPE mapping for these MDBL column types.
		const COLTYPE colType = m_pColumns[iSQLCol].m_eMDBColType;
		ASSERT( (colType != MDCT_CHAR) && (colType != MDCT_IDENTITY) );
		ASSERT( (colType != MDCT_DATETIME) && (colType != MDCT_DATE) && (colType != MDCT_TIME) );
		ASSERT( (colType != MDCT_VOIDPTR) && (colType != MDCT_ROWPTR) && (colType != MDCT_ROWSETPTR) );
#endif

		size_t            iRowCol = m_pColumns[iSQLCol].m_nDstColumn;
		const byte*       pFieldData = pRowData + m_pOffsets[iSQLCol];
		const SQLLEN*     pLenInd = reinterpret_cast<const SQLLEN*>(pFieldData);
		const byte*       pValue = pFieldData + sizeof(SQLLEN);

		// Is value null?
		if (*pLenInd == SQL_NULL_DATA)
		{
			oRow[iRowCol] = null;
		}
		// Requires conversion to MDCT_BOOL?
		else if (m_pColumns[iSQLCol].m_eMDBColType == MDCT_BOOL)
		{
			ASSERT(*pLenInd == 1);

			bool value = *pValue;

			oRow[iRowCol].SetRaw(&value);
		}
		// Requires no conversion.
		else
		{
			oRow[iRowCol].SetRaw(pValue);
		}
	}
}
