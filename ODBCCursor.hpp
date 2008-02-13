/******************************************************************************
**
** MODULE:		ODBCCURSOR.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CODBCCursor class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef ODBCCURSOR_HPP
#define ODBCCURSOR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "SQLCursor.hpp"
#include <sql.h>
#include <sqlext.h>

// Forward declarations.
class CODBCSource;

/******************************************************************************
** 
** This is the SQL cursor type returned for ODBC queries.
**
*******************************************************************************
*/

class CODBCCursor : public CSQLCursor
{
public:
	//
	// Constructors/Destructor.
	//
	CODBCCursor(CODBCSource& oSource);
	virtual ~CODBCCursor();

	virtual void Open(const tchar* pszStmt, SQLHSTMT hStmt);
	virtual void Close();
	
	//
	// Accessors.
	//
	virtual bool IsOpen() const;
	virtual size_t NumColumns() const;
	virtual SQLColumn& Column(size_t n) const;

	virtual bool Fetch();
	virtual void GetRow(CRow& oRow);

protected:
	// Row fetch size.
	enum { FETCH_SIZE = 10 };

	//
	// Members.
	//
	CODBCSource&	m_oSource;		// The data source.
	CString			m_strStmt;		// The statment executed.
	SQLHSTMT		m_hStmt;		// The statement handle.
	size_t			m_nColumns;		// Number of columns returned.
	SQLColumn*		m_pColumns;		// The array of column definitions.
	size_t			m_nRowLen;		// Size of a row.
	size_t			m_nTotalLen;	// Total size of the buffer.
	size_t*			m_pOffsets;		// The array of value offsets.
	byte*			m_pRowData;		// The row data.
	SQLUSMALLINT*	m_pRowStatus;	// The array of status values.
	bool			m_bDoneBind;	// Bind output buffers flag.
	SQLUINTEGER		m_nFetched;		// Number of rows fetched.
	SQLUINTEGER		m_nCurRow;		// Current row 

	//
	// Internal methods.
	//
	virtual void Bind();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //ODBCCURSOR_HPP
