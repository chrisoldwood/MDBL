/******************************************************************************
**
** MODULE:		SQLEXCEPTION.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CSQLException class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SQLEXCEPTION_HPP
#define SQLEXCEPTION_HPP

#if _MSC_VER > 1000
#pragma once
#endif

/******************************************************************************
** 
** This is the exception class thrown by Data Source objects.
**
*******************************************************************************
*/

class CSQLException
{
public:
	//
	// Constructors/Destructor.
	//
	CSQLException(int eErrCode, const tchar* pszSQLStmt, const CString& strSQLErr);
	virtual ~CSQLException();
	
	//
	// Exception codes (0 - 9).
	//
	enum
	{
		E_ENUMINFO_FAILED,	// Failed to enumerate drivers, sources etc.
		E_CONNECT_FAILED,	// Failed to connect.
		E_ALLOC_FAILED,		// Failed to allocate handle/buffer.
		E_EXEC_FAILED,		// Failed to execute query.
		E_FETCH_FAILED,		// Failed to fetch result set data.
		E_TRANS_FAILED,		// Failed to process transaction.
	};

	//
	// Members.
	//
	int		m_eError;		// Error code.
	CString	m_strError;		// Human readable error message.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //SQLEXCEPTION_HPP
