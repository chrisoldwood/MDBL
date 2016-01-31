/******************************************************************************
**
** MODULE:		ODBCEXCEPTION.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CODBCEXception class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ODBCException.hpp"

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

CODBCException::CODBCException(int eErrCode, const tchar* pszSQLStmt, SQLHANDLE hHandle, SQLSMALLINT nType)
	: CSQLException(eErrCode, pszSQLStmt, LastError(hHandle, nType))
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

CODBCException::~CODBCException()
{

}

/******************************************************************************
** Method:		LastError()
**
** Description:	Gets the last error for a SQL handle.
**
** Parameters:	hHandle		The handle to query.
**				nType		The type of handle being queried.
**
** Returns:		The error.
**
*******************************************************************************
*/

CString CODBCException::LastError(SQLHANDLE hHandle, SQLSMALLINT nType)
{
	if (hHandle == NULL)
		return TXT("No error details available.");

	RETCODE		rc = SQL_SUCCESS;
	SQLSMALLINT	nRecord = 1;
	SQLTCHAR	szSqlState[MAX_ERR_BUF_LEN+1] = { 0 };
	SQLINTEGER	nNativeError = 0;
	SQLTCHAR	szErrorMsg[MAX_ERR_BUF_LEN+1] = { 0 };
	SQLSMALLINT	nBufLen = MAX_ERR_BUF_LEN;
	SQLSMALLINT nTxtLen = 0;
	CString		strError;

	while ((rc = ::SQLGetDiagRec(nType, hHandle, nRecord, szSqlState, &nNativeError,
					szErrorMsg, nBufLen, &nTxtLen)) != SQL_NO_DATA_FOUND)
	{
		strError += reinterpret_cast<tchar*>(szSqlState);
		strError += TXT(" - ");
		strError += reinterpret_cast<tchar*>(szErrorMsg);
		strError += TXT("\n");
		++nRecord;
	}

	return strError;
}
