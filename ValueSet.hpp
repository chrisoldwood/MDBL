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

#include <Legacy/TArray.hpp>
#include <WCL/StrArray.hpp>
#include "Value.hpp"

/******************************************************************************
** 
** The class used to store a set of CValues.
**
*******************************************************************************
*/

class CValueSet : protected TPtrArray<CValue>
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
		TPtrArray<CValue>::Add(new CValue(astrStrings[i]));
}

inline CValueSet::CValueSet(const CValueSet& oRHS)
{
	DeepCopy(oRHS);
}

inline CValueSet::~CValueSet()
{
	DeleteAll();
}

inline size_t CValueSet::Count() const
{
	return TPtrArray<CValue>::Size();
}

inline CValue& CValueSet::Value(size_t n) const
{
	return *(TPtrArray<CValue>::At(n));
}

inline CValue& CValueSet::operator[](size_t n) const
{
	return *(TPtrArray<CValue>::At(n));
}

inline size_t CValueSet::Add(const CValue& oValue)
{
	return TPtrArray<CValue>::Add(new CValue(oValue));
}

inline void CValueSet::Delete(size_t nValue)
{
	TPtrArray<CValue>::Delete(nValue);
}

inline void CValueSet::DeleteAll()
{
	TPtrArray<CValue>::DeleteAll();
}

#endif //VALUESET_HPP
