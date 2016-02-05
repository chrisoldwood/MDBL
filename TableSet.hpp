/******************************************************************************
**
** MODULE:		TABLESET.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CTableSet class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef TABLESET_HPP
#define TABLESET_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "FwdDecls.hpp"
#include <Core/Algorithm.hpp>

/******************************************************************************
** 
** The class used to store the tables which belong to a database.
**
*******************************************************************************
*/

class CTableSet : private std::vector<CTable*>
{
public:
	//
	// Constructors/Destructor.
	//
	CTableSet();
	~CTableSet();
	
	//
	// Methods.
	//
	size_t  Count() const;
	CTable& Table(size_t n) const;
	CTable& operator[](size_t n) const;

	size_t Add(CTable& oTable);
	void Remove(size_t nTable);
	void Delete(size_t nTable);

protected:
	//
	// Members.
	//

private:
	//
	// Disallow copy and assignment.
	//
	CTableSet(const CTableSet&);
	void operator=(const CTableSet&);

	//! The underlying collection type.
	typedef std::vector<CTable*> Collection;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline size_t CTableSet::Count() const
{
	return Collection::size();
}

inline CTable& CTableSet::Table(size_t n) const
{
	return *(Collection::operator[](n));
}

inline CTable& CTableSet::operator[](size_t n) const
{
	return *(Collection::operator[](n));
}

inline size_t CTableSet::Add(CTable& oTable)
{
	size_t index = Count();
	Collection::push_back(&oTable);
	return index;
}

inline void CTableSet::Remove(size_t nTable)
{
	Core::eraseAt(*this, nTable);
}

#endif //TABLESET_HPP
