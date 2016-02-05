/******************************************************************************
**
** MODULE:		SQLEXCEPTION.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CSQLException class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "SQLException.hpp"

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	eErrCode	The exception code.
**				strSQLStmt	The statement\connection being executed.
**				strSQLErr	The error returned by the source.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CSQLException::CSQLException(int eErrCode, const tchar* pszSQLStmt, const CString& strSQLErr)
	: Exception()
	, m_eError(eErrCode)
{
	// Convert error to string.
	switch(eErrCode)
	{
		case E_CONNECT_FAILED:	m_details = TXT("Failed to connect to Database:\n\n");		break;
		case E_ALLOC_FAILED:	m_details = TXT("Failed to allocate handle/memory:\n\n");	break;
		case E_EXEC_FAILED:		m_details = TXT("Failed to execute the statement:\n\n");	break;
		case E_FETCH_FAILED:	m_details = TXT("Failed to fetch result set:\n\n");			break;
		case E_TRANS_FAILED:	m_details = TXT("Failed to process transaction:\n\n");		break;
		default:				ASSERT_FALSE();												break;
	}

	// Append statement and reason.
	m_details += pszSQLStmt;
	m_details += TXT("\n\nReason:\n\n");
	m_details += strSQLErr;
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

CSQLException::~CSQLException() throw()
{
}
