/******************************************************************************
**
** MODULE:		JOINEDSET.H
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CJoinedSet class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef JOINEDSET_HPP
#define JOINEDSET_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "ResultSet.hpp"

/******************************************************************************
** 
** This is a 2D array based class used to store the results of a join query.
**
*******************************************************************************
*/

class CJoinedSet
{
public:
	//
	// Constructors/Destructor.
	//
	CJoinedSet(int nTables, CTable* apTables[]);
	CJoinedSet(const CJoinedSet& oJoinedSet);
	~CJoinedSet();
	
	//
	// Methods.
	//
	int         Count() const;
	CResultSet& ResultSet(int n) const;
	CResultSet& operator[](int n) const;

protected:
	//
	// Members.
	//
	int			m_nTables;		// Number of joined tables.
	CResultSet*	m_pResSets;		// Array of result sets.

private:
	//
	// Disallow assignment.
	//
	void operator=(const CJoinedSet&);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline int CJoinedSet::Count() const
{
	ASSERT(m_pResSets != NULL);

	return m_pResSets[0].Count();
}

inline CResultSet& CJoinedSet::ResultSet(int n) const
{
	ASSERT(m_pResSets != NULL);
	ASSERT((n >= 0) && (n < m_nTables));

	return m_pResSets[n];
}

inline CResultSet& CJoinedSet::operator[](int n) const
{
	ASSERT(m_pResSets != NULL);
	ASSERT((n >= 0) && (n < m_nTables));

	return m_pResSets[n];
}

#endif //JOINEDSET_HPP
