/******************************************************************************
**
** MODULE:		ODBCPARAMS.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CODBCParams class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef ODBCPARAMS_HPP
#define ODBCPARAMS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "SQLParams.hpp"
#include <sql.h>
#include <sqlext.h>

// Forward declarations.
class CODBCSource;

/******************************************************************************
**
** This is the SQL parameters type used for ODBC queries.
**
*******************************************************************************
*/

class CODBCParams : public CSQLParams
{
public:
	//
	// Constructors/Destructor.
	//
	CODBCParams(CODBCSource& oSource, const tchar* pszStmt, SQLHSTMT hStmt, size_t nParams);
	virtual ~CODBCParams();

	//
	// Accessors.
	//
	virtual size_t NumParams() const;
	virtual SQLParam& Param(size_t n) const;

	virtual void SetRow(CRow& oRow);

	virtual SQLHSTMT StmtHandle() const;

protected:
	//
	// Members.
	//
	CODBCSource&	m_oSource;		// The data source.
	CString			m_strStmt;		// The statment to be executed.
	SQLHSTMT		m_hStmt;		// The statement handle.
	size_t			m_nParams;		// Number of parameters.
	SQLParam*		m_pParams;		// The array of parameter definitions.
	size_t			m_nRowLen;		// Size of a row.
	size_t*			m_pOffsets;		// The array of value offsets.
	byte*			m_pRowData;		// The row data.
	bool			m_bDoneBind;	// Bind input buffers flag.

	//
	// Internal methods.
	//
	virtual void Bind();

private:
	// NotCopyable.
	CODBCParams(const CODBCParams&);
	CODBCParams& operator=(const CODBCParams&);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline SQLHSTMT CODBCParams::StmtHandle() const
{
	return m_hStmt;
}

#endif //ODBCPARAMS_HPP
