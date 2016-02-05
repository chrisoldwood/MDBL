/******************************************************************************
**
** MODULE:		VALUESET.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CValueSet class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef VALUESET_HPP
#define VALUESET_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/StrArray.hpp>
#include "Value.hpp"
#include <Core/Algorithm.hpp>

/******************************************************************************
**
** The class used to store a set of CValues.
**
*******************************************************************************
*/

class CValueSet : private std::vector<CValue*>
{
public:
	//
	// Constructors/Destructor.
	//
	CValueSet();
	CValueSet(const CStrArray& astrStrings);
	CValueSet(const CValueSet& oRHS);
	~CValueSet();

	//
	// Methods.
	//
	size_t  Count() const;
	CValue& Value(size_t n) const;
	CValue& operator[](size_t n) const;

	size_t Add(const CValue& oValue);
	void  Delete(size_t nValue);
	void  DeleteAll();

protected:
	//
	// Members.
	//

private:
	//
	// Disallow copy and assignment.
	//
	void operator=(const CValueSet&);

	//! The underlying collection type.
	typedef std::vector<CValue*> Collection;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CValueSet::CValueSet()
{
}

inline CValueSet::CValueSet(const CStrArray& astrStrings)
{
	for (size_t i = 0; i < astrStrings.Size(); ++i)
		Collection::push_back(new CValue(astrStrings[i]));
}

inline CValueSet::CValueSet(const CValueSet& oRHS)
    : Collection()
{
	Collection::reserve(oRHS.Count());

	for (size_t i = 0; i < oRHS.Count(); ++i)
		Collection::push_back(new CValue(oRHS[i]));
}

inline CValueSet::~CValueSet()
{
	DeleteAll();
}

inline size_t CValueSet::Count() const
{
	return Collection::size();
}

inline CValue& CValueSet::Value(size_t n) const
{
	return *(Collection::operator[](n));
}

inline CValue& CValueSet::operator[](size_t n) const
{
	return *(Collection::operator[](n));
}

inline size_t CValueSet::Add(const CValue& oValue)
{
	size_t index = Count();
	Collection::push_back(new CValue(oValue));
	return index;
}

inline void CValueSet::Delete(size_t nValue)
{
	Core::deleteAt(*this, nValue);
}

inline void CValueSet::DeleteAll()
{
	Core::deleteAll(*this);
}

#endif //VALUESET_HPP
