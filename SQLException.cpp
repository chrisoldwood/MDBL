/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SQLEXCEPTION.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CSQLException class definition.
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

CSQLException::CSQLException(int eErrCode, const CString& strSQLErr)
{
	m_nErrorCode = eErrCode;

	// Convert error to string.
	switch(eErrCode)
	{
		case E_CONNECT_FAILED:
			m_strErrorText  = "Failed to connect to Database:\n\n";
			m_strErrorText += strSQLErr;
			break;

		case E_EXEC_FAILED:
			m_strErrorText  = "Failed to execute statement:\n\n";
			m_strErrorText += strSQLErr;
			break;

		case E_FETCH_FAILED:
			m_strErrorText  = "Failed to fetch result set:\n\n";
			m_strErrorText += strSQLErr;
			break;

		// Shouldn't happen!
		default:
			ASSERT(false);
			break;
	}
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

CSQLException::~CSQLException()
{
}
