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

#include "MDBLTypes.hpp"

// Forward declarations.
class CRow;

/******************************************************************************
** 
** The type used to return column information.
**
*******************************************************************************
*/

struct SQLColumn
{
	int		m_nDstColumn;		// The destination column.
	CString	m_strName;			// The name.
	int		m_nSQLColType;		// The native SQL column type.
	COLTYPE	m_eMDBColType;		// The nearest MDB column type.
	int		m_nSQLFetchType;	// The SQL type used to fetch the data.
	int		m_nSize;			// The length.
	int		m_nFlags;			// MDB column Flags.
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
	virtual int NumColumns() const = 0;
	virtual SQLColumn& Column(int n) const = 0;

	virtual bool Fetch() = 0;
	virtual void GetRow(CRow& oRow) = 0;

protected:
	//
	// Members.
	//
};

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
