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
	int     Count() const;
	CValue& Value(int n) const;
	CValue& operator[](int n) const;

	int   Add(const CValue& oValue);
	void  Delete(int nValue);
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
	for (int i = 0; i < astrStrings.Size(); ++i)
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

inline int CValueSet::Count() const
{
	return TPtrArray<CValue>::Size();
}

inline CValue& CValueSet::Value(int n) const
{
	return *(TPtrArray<CValue>::At(n));
}

inline CValue& CValueSet::operator[](int n) const
{
	return *(TPtrArray<CValue>::At(n));
}

inline int CValueSet::Add(const CValue& oValue)
{
	return TPtrArray<CValue>::Add(new CValue(oValue));
}

inline void CValueSet::Delete(int nValue)
{
	TPtrArray<CValue>::Delete(nValue);
}

inline void CValueSet::DeleteAll()
{
	TPtrArray<CValue>::DeleteAll();
}

#endif //VALUESET_HPP
