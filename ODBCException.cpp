/******************************************************************************
**
** MODULE:		ODBCEXCEPTION.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CODBCEXception class definition.
**
*******************************************************************************
*/

#include "MDBL.hpp"
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

CODBCException::CODBCException(int eErrCode, const char* pszSQLStmt, SQLHANDLE hHandle, SQLSMALLINT nType)
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

CString CODBCException::LastError(SQLHANDLE hHandle, SQLSMALLINT nType) const
{
	if (hHandle == NULL)
		return "No error details available.";

	RETCODE		rc = SQL_SUCCESS;
	SQLSMALLINT	nRecord = 1;
	SQLCHAR		szSqlState[MAX_ERR_BUF_LEN] = "";
	SQLINTEGER	nNativeError = 0;
	SQLCHAR		szErrorMsg[MAX_ERR_BUF_LEN] = "";
	SQLSMALLINT	nBufLen = MAX_ERR_BUF_LEN - 1;
	SQLSMALLINT nTxtLen = 0;
	CString		strError;

	while ((rc = ::SQLGetDiagRec(nType, hHandle, nRecord, szSqlState, &nNativeError,
					szErrorMsg, nBufLen, &nTxtLen)) != SQL_NO_DATA_FOUND)
	{
		strError += (const char*)szSqlState;
		strError += " - ";
		strError += (const char*)szErrorMsg;
		strError += "\n";
		nRecord++;
	}

	return strError;
}
