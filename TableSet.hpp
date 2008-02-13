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

#include <Legacy/TArray.hpp>
#include "FwdDecls.hpp"

/******************************************************************************
** 
** The class used to store the tables which belong to a database.
**
*******************************************************************************
*/

class CTableSet : protected TPtrArray<CTable>
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
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline size_t CTableSet::Count() const
{
	return Size();
}

inline CTable& CTableSet::Table(size_t n) const
{
	return *(TPtrArray<CTable>::At(n));
}

inline CTable& CTableSet::operator[](size_t n) const
{
	return *(TPtrArray<CTable>::At(n));
}

inline size_t CTableSet::Add(CTable& oTable)
{
	return TPtrArray<CTable>::Add(&oTable);
}

inline void CTableSet::Remove(size_t nTable)
{
	TPtrArray<CTable>::Remove(nTable);
}

inline void CTableSet::Delete(size_t nTable)
{
	TPtrArray<CTable>::Delete(nTable);
}

#endif //TABLESET_HPP
