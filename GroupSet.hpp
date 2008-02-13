/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		GROUPSET.HPP
** COMPONENT:	Memory Database Library
** DESCRIPTION:	The CGroupSet class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef GROUPSET_HPP
#define GROUPSET_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <Legacy/TArray.hpp>
#include "ResultSet.hpp"

/******************************************************************************
** 
** This is a 2D array based class used to store the results of a GROUP BY.
**
*******************************************************************************
*/

class CGroupSet
{
public:
	//
	// Constructors/Destructor.
	//
	CGroupSet();
	CGroupSet(const CGroupSet& oSet);
	~CGroupSet();
	
	//
	// Methods.
	//
	size_t      Count() const;
	CResultSet& ResultSet(size_t n) const;
	CResultSet& operator[](size_t n) const;

	void Add(const CResultSet& oRS);

protected:
	// Template shorthands.
	typedef TPtrArray<CResultSet> CResultSets;

	//
	// Members.
	//
	CResultSets	m_oResSets;

private:
	//
	// Disallow assignment.
	//
	void operator=(const CGroupSet&);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline size_t CGroupSet::Count() const
{
	return m_oResSets.Size();
}

inline CResultSet& CGroupSet::ResultSet(size_t n) const
{
	return *m_oResSets[n];
}

inline CResultSet& CGroupSet::operator[](size_t n) const
{
	return *m_oResSets[n];
}

inline void CGroupSet::Add(const CResultSet& oRS)
{
	m_oResSets.Add(new CResultSet(oRS));
}

#endif // GROUPSET_HPP
