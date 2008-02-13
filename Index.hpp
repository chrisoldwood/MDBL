/******************************************************************************
**
** MODULE:		INDEX.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CIndex class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef INDEX_HPP
#define INDEX_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "ResultSet.hpp"

/******************************************************************************
** 
** This is the base class for all index classes.
**
*******************************************************************************
*/

class CIndex
{
public:
	//
	// Methods.
	//
	size_t Column() const;

	virtual size_t RowCount() const = 0;
	virtual void AddRow(CRow& oRow) = 0;
	virtual void RemoveRow(CRow& oRow) = 0;
	virtual void Truncate() = 0;

	virtual CResultSet FindRows(const CValue& oValue) const = 0;

	virtual void Capacity(size_t nRows) = 0;

protected:
	//
	// Constructors/Destructor.
	//
	CIndex(CTable& oTable, size_t nColumn);
	virtual ~CIndex();
	
	//
	// Members.
	//
	CTable&	m_oTable;		// The parent table.
	size_t	m_nColumn;		// The column to be indexed.

	//
	// Friends.
	//
	friend class CColumn;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CIndex::CIndex(CTable& oTable, size_t nColumn)
	: m_oTable(oTable)
	, m_nColumn(nColumn)
{
}

inline CIndex::~CIndex()
{
}

inline size_t CIndex::Column() const
{
	return m_nColumn;
}

#endif //INDEX_HPP
