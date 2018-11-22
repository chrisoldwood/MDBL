/******************************************************************************
**
** MODULE:		SQLCURSOR.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CSQLCursor class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SQLCURSOR_HPP
#define SQLCURSOR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "FwdDecls.hpp"
#include "MDBLTypes.hpp"

/******************************************************************************
** 
** The type used to return column information.
**
*******************************************************************************
*/

struct SQLColumn
{
	size_t	m_nDstColumn;		// The destination column.
	CString	m_strName;			// The name.
	COLTYPE	m_eMDBColType;		// The nearest MDB column type.
	size_t	m_nSize;			// The length.
	uint	m_nFlags;			// MDB column Flags.

	SQLColumn()
	 : m_nDstColumn(), m_strName(), m_eMDBColType(), m_nSize(), m_nFlags()
	{}

	SQLColumn(size_t index, CString name, COLTYPE type, size_t size, uint flags)
	 : m_nDstColumn(index), m_strName(name), m_eMDBColType(type), m_nSize(size), m_nFlags(flags)
	{}

protected:
	// Make abstract
	~SQLColumn() {};
};

/******************************************************************************
** 
** This is the base class for SQL result cursors.
**
*******************************************************************************
*/

class CSQLCursor
{
public:
	//
	// Constructors/Destructor.
	//
	CSQLCursor();
	virtual ~CSQLCursor();
	
	//
	// Accessors.
	//
	virtual size_t NumColumns() const = 0;
	virtual const SQLColumn& Column(size_t n) const = 0;

	//! Set the mapping from result set column to table column.
	virtual void MapColumn(size_t sourceColumn, size_t destColumn, COLTYPE type, size_t size) = 0;

	virtual bool Fetch() = 0;
	virtual void GetRow(CRow& oRow) = 0;

protected:
	//
	// Members.
	//
};

//! The default SQL cursor smart pointer type.
typedef Core::SharedPtr<CSQLCursor> SQLCursorPtr;

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CSQLCursor::CSQLCursor()
{
}

inline CSQLCursor::~CSQLCursor()
{
}

#endif //SQLCURSOR_HPP
