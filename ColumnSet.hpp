/******************************************************************************
**
** MODULE:		COLUMNSET.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CColumnSet class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef COLUMNSET_HPP
#define COLUMNSET_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Column.hpp"
#include <Legacy/TArray.hpp>

/******************************************************************************
** 
** The class used to store the columns which belong to a table.
**
*******************************************************************************
*/

class CColumnSet : protected TPtrArray<CColumn>
{
public:
	//
	// Constructors/Destructor.
	//
	CColumnSet();
	~CColumnSet();
	
	//
	// Methods.
	//
	size_t   Count() const;
	CColumn& Column(size_t n) const;
	CColumn& operator[](size_t n) const;

	size_t Add(CColumn& oColumn);
	void Delete(size_t n);
	void DeleteAll();
	int  Find(const tchar* pszName);

	size_t AllocSize() const;

protected:
	//
	// Members.
	//

private:
	//
	// Disallow copy and assignment.
	//
	CColumnSet(const CColumnSet&);
	void operator=(const CColumnSet&);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline size_t CColumnSet::Count() const
{
	return TPtrArray<CColumn>::Size();
}

inline CColumn& CColumnSet::Column(size_t n) const
{
	return *(TPtrArray<CColumn>::At(n));
}

inline CColumn& CColumnSet::operator[](size_t n) const
{
	return *(TPtrArray<CColumn>::At(n));
}

inline size_t CColumnSet::Add(CColumn& oColumn)
{
	return TPtrArray<CColumn>::Add(&oColumn);
}

#endif //COLUMNSET_HPP
