/******************************************************************************
**
** MODULE:		ODBCSOURCE.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CODBCSource class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ODBCSource.hpp"
#include <odbcinst.h>
#include "ODBCException.hpp"
#include "ODBCParams.hpp"
#include "ODBCCursor.hpp"
#include "TimeStamp.hpp"
#include <WCL/StrArray.hpp>

// Linker directives.
#pragma comment(lib, "odbc32")
#pragma comment(lib, "odbccp32")

/******************************************************************************
**
** Constants.
**
*******************************************************************************
*/

// Maximum length for connection string.
const int MAX_CONN_STR_LEN = 1024;

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
	, m_bInTrans(false)
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

void CODBCSource::Open(const tchar* pszConnection)
{
	ASSERT(IsOpen()  == false);
	ASSERT(InTrans() == false);

	SQLRETURN	rc;
	SQLTCHAR	szConnection[MAX_CONN_STR_LEN+1] = { 0 };
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

	SQLTCHAR* pszPartialConnStr = reinterpret_cast<SQLTCHAR*>(const_cast<tchar*>(pszConnection));

	// Connect to the database.
	rc = ::SQLDriverConnect(m_hDBC, NULL, pszPartialConnStr, SQL_NTS,
							szConnection, MAX_CONN_STR_LEN, &nLength,
							SQL_DRIVER_NOPROMPT);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_CONNECT_FAILED, pszConnection, m_hDBC, SQL_HANDLE_DBC);

	ASSERT(m_hEnv != SQL_NULL_HENV);
	ASSERT(m_hDBC != SQL_NULL_HDBC);
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

	// Reset members.
	m_bInTrans = false;
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

void CODBCSource::ExecStmt(const tchar* pszStmt)
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

		SQLTCHAR* ptszStmt = reinterpret_cast<SQLTCHAR*>(const_cast<tchar*>(pszStmt));

		// Execute the query.
		rc = ::SQLExecDirect(hStmt, ptszStmt, SQL_NTS);

		if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
			throw CODBCException(CODBCException::E_EXEC_FAILED, pszStmt, hStmt, SQL_HANDLE_STMT);

		// Free statement handle.
		::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
	catch (const CODBCException&)
	{
		// Free statement handle.
		::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		throw;
	}
}

/******************************************************************************
** Method:		ExecStmt()
**
** Description:	Executes the given parameterised statement.
**
** Parameters:	pszStmt		The SQL statement.
**				oParams		The parameters.
**
** Returns:		Nothing.
**
** Exceptions:	CODBCException on error.
**
*******************************************************************************
*/

void CODBCSource::ExecStmt(const tchar* pszStmt, CSQLParams& oParams)
{
	ASSERT(IsOpen() == true);

	// Downcast to get real parameters type.
	CODBCParams& oODBCParams = static_cast<CODBCParams&>(oParams);

	SQLRETURN	rc;
	SQLHSTMT	hStmt = oODBCParams.StmtHandle();

	ASSERT(hStmt != SQL_NULL_HSTMT);

	SQLTCHAR* ptszStmt = reinterpret_cast<SQLTCHAR*>(const_cast<tchar*>(pszStmt));

	// Execute the query.
	rc = ::SQLExecDirect(hStmt, ptszStmt, SQL_NTS);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_EXEC_FAILED, pszStmt, hStmt, SQL_HANDLE_STMT);
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

CSQLCursor* CODBCSource::ExecQuery(const tchar* pszQuery)
{
	ASSERT(IsOpen() == true);

	// Allocate a cursor for the result set.
	CODBCCursor* pCursor = new CODBCCursor(*this);

	try
	{
		// Execute it.
		ExecQuery(pszQuery, *pCursor);
	}
	catch (const CODBCException&)
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

void CODBCSource::ExecQuery(const tchar* pszQuery, CODBCCursor& oCursor)
{
	ASSERT(IsOpen() == true);

	SQLRETURN	rc;
	SQLHSTMT	hStmt = SQL_NULL_HSTMT;

	// Allocate a statement handle.
	rc = ::SQLAllocHandle(SQL_HANDLE_STMT, m_hDBC, &hStmt);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_EXEC_FAILED, pszQuery, m_hDBC, SQL_HANDLE_DBC);

	ASSERT(hStmt != SQL_NULL_HSTMT);

	SQLTCHAR* ptszQuery = reinterpret_cast<SQLTCHAR*>(const_cast<tchar*>(pszQuery));

	// Execute the query.
	rc = ::SQLExecDirect(hStmt, ptszQuery, SQL_NTS);

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
		case SQL_BIT:				return MDCT_BOOL;

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

	ASSERT_FALSE();

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
		case MDCT_BOOL:			return SQL_BIT;
		case MDCT_IDENTITY:		return SQL_INTEGER;
		case MDCT_DATETIME:		return SQL_TYPE_TIMESTAMP;
		case MDCT_DATE:			return SQL_TYPE_TIMESTAMP;
		case MDCT_TIME:			return SQL_TYPE_TIMESTAMP;
		case MDCT_TIMESTAMP:	return SQL_TYPE_TIMESTAMP;
	}

	ASSERT_FALSE();

	// Unsupported.
	return SQL_UNKNOWN_TYPE;
}

/******************************************************************************
** Method:		BufferSize()
**
** Description:	Calculates the buffer size required for the data type.
**
** Parameters:	eType	The MDB column type.
**				nSize	The column size.
**
** Returns:		The buffer size.
**
*******************************************************************************
*/

size_t CODBCSource::BufferSize(COLTYPE eColType, size_t nColSize)
{
	size_t nSize = 0;

	switch (eColType)
	{
		case MDCT_INT:			nSize = sizeof(int);			break;
		case MDCT_DOUBLE:		nSize = sizeof(double);			break;
		case MDCT_CHAR:			nSize = Core::NumBytes<char>(1+1);			break;	// ANSI only.
		case MDCT_FXDSTR:		nSize = Core::NumBytes<char>(nColSize+1);	break;	// ANSI only.
		case MDCT_VARSTR:		nSize = Core::NumBytes<char>(nColSize+1);	break;	// ANSI only.
		case MDCT_BOOL:			nSize = sizeof(bool);			break;
		case MDCT_IDENTITY:		nSize = sizeof(int);			break;
		case MDCT_DATETIME:		nSize = sizeof(CTimeStamp);		break;
		case MDCT_DATE:			nSize = sizeof(CTimeStamp);		break;
		case MDCT_TIME:			nSize = sizeof(CTimeStamp);		break;
		case MDCT_TIMESTAMP:	nSize = sizeof(CTimeStamp);		break;
		default:				ASSERT_FALSE();					break;
	}

	// Adjust size for alignment.
	nSize = (nSize + 3) & ~3;

	return nSize;
}

/******************************************************************************
** Method:		ColumnSize()
**
** Description:	Calculates the SQL column size for the data type.
**				NB: smalldatetime is 16 datetime is 23
**
** Parameters:	eType	The MDB column type.
**				nSize	The column size.
**
** Returns:		The column size.
**
*******************************************************************************
*/

size_t CODBCSource::ColumnSize(COLTYPE eColType, size_t nColSize)
{
	switch (eColType)
	{
		case MDCT_INT:			return 0;
		case MDCT_DOUBLE:		return 0;
		case MDCT_CHAR:			return 1;
		case MDCT_FXDSTR:		return nColSize;
		case MDCT_VARSTR:		return nColSize;
		case MDCT_BOOL:			return 0;
		case MDCT_IDENTITY:		return 0;
		case MDCT_DATETIME:		return 23;
		case MDCT_DATE:			return 23;
		case MDCT_TIME:			return 23;
		case MDCT_TIMESTAMP:	return 23;
	}

	ASSERT_FALSE();

	// Unsupported.
	return 0;
}

/******************************************************************************
** Method:		CreateParams()
**
** Description:	Creates a parameters object for use with a parameterised
**				statement.
**
** Parameters:	pszStmt		The statment that will be executed.
**				nParams		The expected number of parameters.
**
** Returns:		The params object.
**
*******************************************************************************
*/

CSQLParams* CODBCSource::CreateParams(const tchar* pszStmt, size_t nParams)
{
	SQLHSTMT hStmt = SQL_NULL_HSTMT;

	// Allocate a statement handle.
	SQLRETURN rc = ::SQLAllocHandle(SQL_HANDLE_STMT, m_hDBC, &hStmt);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_ALLOC_FAILED, pszStmt, m_hDBC, SQL_HANDLE_DBC);

	ASSERT(hStmt != SQL_NULL_HSTMT);

	return new CODBCParams(*this, pszStmt, hStmt, nParams);
}

/******************************************************************************
** Method:		InTrans()
**
** Description:	Queries if the connection is in a transaction.
**
** Parameters:	None.
**
** Returns:		true or false.
**
** Exceptions:	CODBCException on error.
**
*******************************************************************************
*/

bool CODBCSource::InTrans()
{
	return m_bInTrans;
}

/******************************************************************************
** Method:		BeginTrans()
**
** Description:	Start a new transaction.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
** Exceptions:	CODBCException on error.
**
*******************************************************************************
*/

void CODBCSource::BeginTrans()
{
	ASSERT(m_bInTrans == false);

	// Start the transaction.
	SQLRETURN rc = ::SQLSetConnectAttr(m_hDBC, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_IS_UINTEGER);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_TRANS_FAILED, TXT("Begin Transaction"), m_hDBC, SQL_HANDLE_DBC);

	m_bInTrans = true;
}

/******************************************************************************
** Method:		CommitTrans()
**
** Description:	Commit the current transaction.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
** Exceptions:	CODBCException on error.
**
*******************************************************************************
*/

void CODBCSource::CommitTrans()
{
	ASSERT(m_bInTrans == true);

	SQLRETURN rc;

	// Commit the transaction.
	rc = ::SQLEndTran(SQL_HANDLE_DBC, m_hDBC, SQL_COMMIT);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_TRANS_FAILED, TXT("Commit Transaction"), m_hDBC, SQL_HANDLE_DBC);

	// End the transaction.
	rc = ::SQLSetConnectAttr(m_hDBC, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_UINTEGER);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_TRANS_FAILED, TXT("End Transaction"), m_hDBC, SQL_HANDLE_DBC);

	m_bInTrans = false;
}

/******************************************************************************
** Method:		Trans()
**
** Description:	Rollback the current transaction.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
** Exceptions:	CODBCException on error.
**
*******************************************************************************
*/

void CODBCSource::RollbackTrans()
{
	ASSERT(m_bInTrans == true);

	SQLRETURN rc;

	// Rollback transaction.
	rc = ::SQLEndTran(SQL_HANDLE_DBC, m_hDBC, SQL_ROLLBACK);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_TRANS_FAILED, TXT("Rollback Transaction"), m_hDBC, SQL_HANDLE_DBC);

	// End the transaction.
	rc = ::SQLSetConnectAttr(m_hDBC, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_UINTEGER);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_TRANS_FAILED, TXT("End Transaction"), m_hDBC, SQL_HANDLE_DBC);

	m_bInTrans = false;
}

/******************************************************************************
** Method:		InstalledDrivers()
**
** Description:	Gets a list of the currently installed drivers.
**
** Parameters:	astrDrivers		The return buffer for the list of drivers.
**
** Returns:		Nothing.
**
** Exceptions:	CODBCException on error.
**
*******************************************************************************
*/

void CODBCSource::InstalledDrivers(CStrArray& astrDrivers)
{
	size_t nChars    = 1024;
	WORD   wRetSize  = 0;
	tchar* pszBuffer = NULL;

	// Until buffer big enough.
	for(;;)
	{
		size_t nBytes = Core::NumBytes<tchar>(nChars);

		pszBuffer = static_cast<tchar*>(alloca(nBytes));

		// Try query.
		if (!::SQLGetInstalledDrivers(pszBuffer, static_cast<WORD>(nBytes), &wRetSize))
			throw CODBCException(CODBCException::E_ENUMINFO_FAILED, TXT("SQLGetInstalledDrivers"), NULL, 0);

		// Buffer big enough?
		if (wRetSize < nChars)
			break;

		// Double buffer size, and try again.
		nChars *= 2;
	}

	ASSERT(pszBuffer != NULL);

	tchar* pszName = pszBuffer;

	// Extract drivers.
	while (*pszName != TXT('\0'))
	{
		astrDrivers.Add(pszName);

		while (*pszName++ != TXT('\0'));
	}
}

/******************************************************************************
** Method:		InstalledSources()
**
** Description:	Gets a list of the currently installed data sources (DSNs).
**
** Parameters:	astrSources		The return buffer for the list of DSNs.
**
** Returns:		Nothing.
**
** Exceptions:	CODBCException on error.
**
*******************************************************************************
*/

void CODBCSource::InstalledSources(CStrArray& astrSources)
{
	const size_t MAX_DSN_LEN  = 256;
	const size_t MAX_DESC_LEN = 256;

	SQLHENV		hEnv = SQL_NULL_HENV;
	SQLRETURN	rc;
	SQLTCHAR	szDSN[MAX_DSN_LEN+1] = { 0 };
	SQLTCHAR	szDesc[MAX_DESC_LEN+1] = { 0 };
	SQLSMALLINT	nDSNSize  = MAX_DSN_LEN;
	SQLSMALLINT	nDescSize = MAX_DESC_LEN;
	SQLSMALLINT	nDSNRetSize;
	SQLSMALLINT	nDescRetSize;
	
	// Allocate an environment handle.
	rc = ::SQLAllocHandle(SQL_HANDLE_ENV, NULL, &hEnv);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_ENUMINFO_FAILED, TXT("SQLAllocHandle"), NULL, 0);

	// Say we're ODBC v3.x compliant.
	rc = ::SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) )
		throw CODBCException(CODBCException::E_CONNECT_FAILED, TXT("SQLSetEnvAttr"), hEnv, SQL_HANDLE_ENV);

	// Fetch the first data source.
	rc = ::SQLDataSources(hEnv, SQL_FETCH_FIRST, szDSN, nDSNSize, &nDSNRetSize, szDesc, nDescSize, &nDescRetSize);

	if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) && (rc != SQL_NO_DATA) )
	{
		::SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		throw CODBCException(CODBCException::E_ENUMINFO_FAILED, TXT("SQLDataSources"), hEnv, SQL_HANDLE_ENV);
	}

	// For all sources.
	while (rc != SQL_NO_DATA)
	{
		astrSources.Add(reinterpret_cast<tchar*>(szDSN));

		rc = ::SQLDataSources(hEnv, SQL_FETCH_NEXT, szDSN, nDSNSize, &nDSNRetSize, szDesc, nDescSize, &nDescRetSize);

		if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) && (rc != SQL_NO_DATA) )
		{
			::SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
			throw CODBCException(CODBCException::E_ENUMINFO_FAILED, TXT("SQLDataSources"), hEnv, SQL_HANDLE_ENV);
		}
	}

	// Free environment.
	::SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}
