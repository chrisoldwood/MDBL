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
** The type used to return column information.
**
*******************************************************************************
*/

struct ODBCColumn : public SQLColumn
{
	int		m_nSQLColType;		// The native SQL column type.
	int		m_nSQLFetchType;	// The SQL type used to fetch the data.
};

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
	virtual const SQLColumn& Column(size_t n) const;

	//! Set the mapping from result set column to table column.
	virtual void MapColumn(size_t sourceColumn, size_t destColumn, COLTYPE type, size_t size);

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
	ODBCColumn*		m_pColumns;		// The array of column definitions.
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

private:
	// NotCopyable.
	CODBCCursor(const CODBCCursor&);
	CODBCCursor& operator=(const CODBCCursor&);
};

//! The default ODBC cursor smart pointer type.
typedef Core::SharedPtr<CODBCCursor> ODBCCursorPtr;

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //ODBCCURSOR_HPP
