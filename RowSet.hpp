/******************************************************************************
**
** MODULE:		ROWSET.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CRowSet class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef ROWSET_HPP
#define ROWSET_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <Legacy/TArray.hpp>
#include "Row.hpp"

/******************************************************************************
** 
** The class used to store the rows which belong to a table.
**
*******************************************************************************
*/

class CRowSet : protected TPtrArray<CRow>
{
public:
	//
	// Constructors/Destructor.
	//
	CRowSet();
	~CRowSet();
	
	//
	// Methods.
	//
	size_t Count() const;
	CRow& Row(size_t n) const;
	CRow& operator[](size_t n) const;

	size_t Add(CRow& oRow);
	void  Remove(size_t nRow);

	void  Delete(size_t nRow);
	void  DeleteAll();

	bool  Modified() const;

private:
	//
	// Disallow copy and assignment.
	//
	CRowSet(const CRowSet&);
	void operator=(const CRowSet&);

	//
	// Friends.
	//
	friend class CResultSet;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CRowSet::CRowSet()
{
}

inline CRowSet::~CRowSet()
{
	DeleteAll();
}

inline size_t CRowSet::Count() const
{
	return TPtrArray<CRow>::Size();
}

inline CRow& CRowSet::Row(size_t n) const
{
	return *(TPtrArray<CRow>::At(n));
}

inline CRow& CRowSet::operator[](size_t n) const
{
	return *(TPtrArray<CRow>::At(n));
}

inline size_t CRowSet::Add(CRow& oRow)
{
	return TPtrArray<CRow>::Add(&oRow);
}

inline void CRowSet::Remove(size_t nRow)
{
	TPtrArray<CRow>::Remove(nRow);
}

inline void CRowSet::Delete(size_t nRow)
{
	TPtrArray<CRow>::Delete(nRow);
}

inline void CRowSet::DeleteAll()
{
	TPtrArray<CRow>::DeleteAll();
}

inline bool CRowSet::Modified() const
{
	for (size_t i = 0; i < Count(); ++i)
	{
		if (Row(i).Modified())
			return true;
	}

	return false;
}

#endif //ROWSET_HPP
