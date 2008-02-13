/******************************************************************************
**
** MODULE:		ODBCEXCEPTION.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CODBCException class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef ODBCEXCEPTION_HPP
#define ODBCEXCEPTION_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "SQLException.hpp"
#include <sql.h>
#include <sqlext.h>

/******************************************************************************
** 
** This is the exception class thrown by ODBC data source objects.
**
*******************************************************************************
*/

class CODBCException : public CSQLException  
{
public:
	//
	// Constructors/Destructor.
	//
	CODBCException(int eErrCode, const tchar* pszSQLStmt, SQLHANDLE hHandle, SQLSMALLINT nType);
	virtual ~CODBCException();

protected:
	// Max size of error buffer.
	enum { MAX_ERR_BUF_LEN = 256 };

	//
	// Internal methods.
	//
	CString LastError(SQLHANDLE hHandle, SQLSMALLINT nType) const;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // ODBCEXCEPTION_HPP
