/******************************************************************************
**
** MODULE:		ODBCSOURCE.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CODBCSource class definition.
**
*******************************************************************************
*/

#include "MDBL.hpp"

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CODBCSource::CODBCSource()
	: m_hEnv(SQL_NULL_HENV)
	, m_hDBC(SQL_NULL_HDBC)
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

CODBCSource::~CODBCSource()
{
	Close();
}

/******************************************************************************
** Method:		Open()
**
** Description:	Opens a connection to the database.
**
** Parameters:	pszConnection	The connection string.
**
** Returns:		Nothing.
**
** Exceptions:	CODBCException on error.
**
*******************************************************************************
*/

void CODBCSource::Open(const char* pszConnection)
{
	ASSERT(IsOpen() == false);

	SQLRETURN	rc;
	SQLCHAR		szConnection[MAX_PATH];
	SQLSMALLINT	nLength = 0;

	// Allocate the environment handle.
	rc = ::SQLAllocHandle(SQL_HANDLE_ENV, NULL, &m_hEnv);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_CONNECT_FAILED, pszConnection, NULL, 0);

	// Say we're ODBC v3.x compliant.
	rc = ::SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_CONNECT_FAILED, pszConnection, m_hEnv, SQL_HANDLE_ENV);

	// Allocate the connection handle.
	rc = ::SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDBC);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_CONNECT_FAILED, pszConnection, m_hEnv, SQL_HANDLE_ENV);

	// Connect to the database.
	rc = ::SQLDriverConnect(m_hDBC, NULL, (SQLCHAR*)pszConnection, SQL_NTS,
							(SQLCHAR*)&szConnection, MAX_PATH, &nLength,
							SQL_DRIVER_NOPROMPT);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_CONNECT_FAILED, pszConnection, m_hDBC, SQL_HANDLE_DBC);

	ASSERT(m_hEnv  != SQL_NULL_HENV);
	ASSERT(m_hDBC  != SQL_NULL_HDBC);
}

/******************************************************************************
** Method:		Close()
**
** Description:	Closes the connection to the database.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CODBCSource::Close()
{
	// Free connection?
	if (m_hDBC != SQL_NULL_HDBC)
	{
		::SQLDisconnect(m_hDBC);
		::SQLFreeHandle(SQL_HANDLE_DBC, m_hDBC);

		m_hDBC = SQL_NULL_HDBC;
	}

	// Free environment?
	if (m_hEnv != SQL_NULL_HENV)
	{
		::SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);

		m_hEnv = SQL_NULL_HENV;
	}
}

/******************************************************************************
** Method:		IsOpen()
**
** Description:	Checks if the database connection is open.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CODBCSource::IsOpen() const
{
	return ( (m_hDBC != SQL_NULL_HDBC) && (m_hEnv != SQL_NULL_HENV) );
}

/******************************************************************************
** Method:		ExecStmt()
**
** Description:	Executes the given statement.
**
** Parameters:	pszStmt		The SQL statement.
**
** Returns:		Nothing.
**
** Exceptions:	CODBCException on error.
**
*******************************************************************************
*/

void CODBCSource::ExecStmt(const char* pszStmt)
{
	ASSERT(IsOpen() == true);

	SQLRETURN	rc;
	SQLHSTMT	hStmt = SQL_NULL_HSTMT;

	try
	{
		// Allocate a statement handle.
		rc = ::SQLAllocHandle(SQL_HANDLE_STMT, m_hDBC, &hStmt);

		if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
			throw CODBCException(CODBCException::E_EXEC_FAILED, pszStmt, m_hDBC, SQL_HANDLE_DBC);

		ASSERT(hStmt != SQL_NULL_HSTMT);

		// Execute the query.
		rc = ::SQLExecDirect(hStmt, (SQLCHAR*)pszStmt, SQL_NTS);

		if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
			throw CODBCException(CODBCException::E_EXEC_FAILED, pszStmt, hStmt, SQL_HANDLE_STMT);

		// Free statement handle.
		::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
	catch(CODBCException&)
	{
		// Free statement handle.
		::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		throw;
	}
}

/******************************************************************************
** Method:		ExecQuery()
**
** Description:	Executes the given query and returns a cursor for the result
**				set.
**
** Parameters:	pszQuery	The SQL query statement.
**
** Returns:		A result set cursor.
**
** Exceptions:	CODBCException on error.
**
*******************************************************************************
*/

CSQLCursor* CODBCSource::ExecQuery(const char* pszQuery)
{
	ASSERT(IsOpen() == true);

	// Allocate a cursor for the result set.
	CODBCCursor* pCursor = new CODBCCursor(*this);

	try
	{
		// Execute it.
		ExecQuery(pszQuery, *pCursor);
	}
	catch(CODBCException&)
	{
		// Free cursor and re-throw exception.
		delete pCursor;
		throw;
	}

	return pCursor;
}

/******************************************************************************
** Method:		ExecQuery()
**
** Description:	Executes the given query and stores the result set in the given
**				cursor.
**
** Parameters:	pszQuery	The SQL query statement.
**				oCursor		The cursor to use for the result set.
**
** Returns:		Nothing.
**
** Exceptions:	CODBCException on error.
**
*******************************************************************************
*/

void CODBCSource::ExecQuery(const char* pszQuery, CODBCCursor& oCursor)
{
	ASSERT(IsOpen() == true);

	SQLRETURN	rc;
	SQLHSTMT	hStmt = SQL_NULL_HSTMT;

	// Allocate a statement handle.
	rc = ::SQLAllocHandle(SQL_HANDLE_STMT, m_hDBC, &hStmt);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_EXEC_FAILED, pszQuery, m_hDBC, SQL_HANDLE_DBC);

	ASSERT(hStmt != SQL_NULL_HSTMT);

	// Execute the query.
	rc = ::SQLExecDirect(hStmt, (SQLCHAR*)pszQuery, SQL_NTS);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_EXEC_FAILED, pszQuery, hStmt, SQL_HANDLE_STMT);

	oCursor.Open(pszQuery, hStmt);
}

/******************************************************************************
** Method:		MDBType()
**
** Description:	Gets the MDB column type that corresponds the ODBC type.
**
** Parameters:	nODBCType	The ODBC type.
**
** Returns:		The MDB column type.
**
*******************************************************************************
*/

COLTYPE CODBCSource::MDBType(SQLSMALLINT nODBCType)
{
	switch (nODBCType)
	{
		// Special types.
		case SQL_UNKNOWN_TYPE:		break;

		// Character types.
		case SQL_CHAR:				return MDCT_FXDSTR;
		case SQL_VARCHAR:			return MDCT_FXDSTR;
		case SQL_LONGVARCHAR:		break;
		case SQL_WCHAR:				return MDCT_FXDSTR;
		case SQL_WVARCHAR:			return MDCT_FXDSTR;
		case SQL_WLONGVARCHAR:		break;

		// Floating point types.
		case SQL_NUMERIC:			return MDCT_DOUBLE;
		case SQL_DECIMAL:			return MDCT_DOUBLE;
		case SQL_REAL:				break;
		case SQL_FLOAT:				return MDCT_DOUBLE;
		case SQL_DOUBLE:			return MDCT_DOUBLE;

		// Integer types.
		case SQL_INTEGER:			return MDCT_INT;
		case SQL_SMALLINT:			return MDCT_INT;
		case SQL_TINYINT:			return MDCT_INT;
		case SQL_BIGINT:			break;

		// Boolean types.
		case SQL_BIT:				break;

		// Date/Time types.
		case SQL_DATE:				break;
		case SQL_TIME:				break;
		case SQL_TIMESTAMP:			break;
		case SQL_TYPE_DATE:			return MDCT_TIMESTAMP;
		case SQL_TYPE_TIME:			return MDCT_TIMESTAMP;
		case SQL_TYPE_TIMESTAMP:	return MDCT_TIMESTAMP;

		// Blob types.
		case SQL_BINARY:			break;
		case SQL_VARBINARY:			break;
		case SQL_LONGVARBINARY:		break;

		// Unknown.
		default:					break;
	}

	ASSERT(false);

	// Unsupported.
	return MDCT_FXDSTR;
}

/******************************************************************************
** Method:		ODBCType()
**
** Description:	Gets the ODBC type that corresponds the MDB column type.
**
** Parameters:	nMDBType	The MDB column type.
**
** Returns:		The ODBC type.
**
*******************************************************************************
*/

SQLSMALLINT CODBCSource::ODBCType(COLTYPE eMDBType)
{
	switch (eMDBType)
	{
		case MDCT_INT:			return SQL_INTEGER;
		case MDCT_DOUBLE:		return SQL_DOUBLE;
		case MDCT_CHAR:			return SQL_CHAR;
		case MDCT_FXDSTR:		return SQL_CHAR;
		case MDCT_VARSTR:		return SQL_CHAR;
		case MDCT_BOOL:			break;
		case MDCT_IDENTITY:		return SQL_INTEGER;
		case MDCT_DATETIME:		return SQL_TYPE_TIMESTAMP;
		case MDCT_TIMESTAMP:	return SQL_TYPE_TIMESTAMP;
	}

	ASSERT(false);

	// Unsupported.
	return SQL_UNKNOWN_TYPE;
}

/******************************************************************************
** Method:		BufferSize()
**
** Description:	Calculates the buffer size required for the data type.
**
** Parameters:	oColumn		The column definition.
**
** Returns:		The buffer size.
**
*******************************************************************************
*/

int CODBCSource::BufferSize(const SQLColumn& oColumn)
{
	int nSize = 0;

	switch (oColumn.m_eMDBColType)
	{
		case MDCT_INT:			nSize = sizeof(int);			break;
		case MDCT_DOUBLE:		nSize = sizeof(double);			break;
		case MDCT_CHAR:			nSize = sizeof(char) + 1;		break;
		case MDCT_FXDSTR:		nSize = oColumn.m_nSize + 1;	break;
		case MDCT_VARSTR:		nSize = oColumn.m_nSize + 1;	break;
		case MDCT_BOOL:			ASSERT(false);					break;
		case MDCT_IDENTITY:		nSize = sizeof(int);			break;
		case MDCT_DATETIME:		nSize = sizeof(CTimeStamp);		break;
		case MDCT_TIMESTAMP:	nSize = sizeof(CTimeStamp);		break;
	}

	// Adjust size for alignment.
	nSize = (nSize + 3) & ~3;

	// Unsupported.
	return nSize;
}
