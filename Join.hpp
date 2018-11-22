/******************************************************************************
**
** MODULE:		JOIN.H
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CJoin class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef JOIN_HPP
#define JOIN_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <limits>

////////////////////////////////////////////////////////////////////////////////
//! The type of join.

enum JoinType
{
	INNER_JOIN,
	OUTER_JOIN,
};

/******************************************************************************
**
** The class used to hold a table used in a CJoin query.
**
*******************************************************************************
*/

class CJoinTable
{
public:
	//
	// Constructors/Destructor.
	//
	CJoinTable(size_t nTable, size_t nLHSColumn, JoinType eJoinType, size_t nRHSColumn);
	~CJoinTable();

	//
	// Members.
	//
	size_t	m_nTable;		// The table to join onto.
	size_t	m_nLHSColumn;	// The left hand tables' column.
	JoinType m_eJoinType;	// The join type (INNER or OUTER).
	size_t	m_nRHSColumn;	// The right hand tables' column.
};

/******************************************************************************
**
** This class is used to hold the query when doing a SELECT involving joins.
**
*******************************************************************************
*/

class CJoin : private std::vector<CJoinTable*>
{
public:
	//
	// Constructors/Destructor.
	//
	CJoin(size_t nTable);
	~CJoin();

	//
	// Methods.
	//
	size_t      Count() const;
	CJoinTable& Join(size_t n) const;
	CJoinTable& operator[](size_t n) const;

	void Add(size_t nTable, size_t nLHSColumn, JoinType eJoinType, size_t nRHSColumn);

protected:
	//
	// Members.
	//

	//! The underlying collection type.
	typedef std::vector<CJoinTable*> Collection;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CJoin::CJoin(size_t nTable)
{
	const size_t anyColumn = std::numeric_limits<size_t>::max();
	const JoinType anyJoinType = INNER_JOIN;

	// The leftmost table is implicitly joined.
	Add(nTable, anyColumn, anyJoinType, anyColumn);
}

inline CJoin::~CJoin()
{
	for (size_t i = 0; i < Count(); ++i)
		delete &Join(i);

	Collection::clear();
}

inline size_t CJoin::Count() const
{
	return Collection::size();
}

inline CJoinTable& CJoin::Join(size_t n) const
{
	ASSERT(n < Count());

	return *(Collection::operator[](n));
}

inline CJoinTable& CJoin::operator[](size_t n) const
{
	ASSERT(n < Count());

	return *(Collection::operator[](n));
}

inline void CJoin::Add(size_t nTable, size_t nLHSColumn, JoinType eJoinType, size_t nRHSColumn)
{
	Collection::push_back(new CJoinTable(nTable, nLHSColumn, eJoinType, nRHSColumn));
}

inline CJoinTable::CJoinTable(size_t nTable, size_t nLHSColumn, JoinType eJoinType, size_t nRHSColumn)
	: m_nTable(nTable)
	, m_nLHSColumn(nLHSColumn)
	, m_eJoinType(eJoinType)
	, m_nRHSColumn(nRHSColumn)
{
}

inline CJoinTable::~CJoinTable()
{
}

#endif //JOIN_HPP
