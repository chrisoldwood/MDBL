/******************************************************************************
** (C) Chris Oldwood
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

/******************************************************************************
** 
** The class used to store the columns which belong to a table.
**
*******************************************************************************
*/

class CColumnSet : public CPtrArray
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
	CColumn& Column(int n) const;
	CColumn& operator[](int n) const;

	int  Add(CColumn& oColumn);
	void Delete(int n);
	void DeleteAll();
	int  Find(const char* pszName);

	int AllocSize() const;

protected:
	//
	// Members.
	//
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CColumn& CColumnSet::Column(int n) const
{
	return *((CColumn*) CPtrArray::Item(n));
}

inline CColumn& CColumnSet::operator[](int n) const
{
	return *((CColumn*) CPtrArray::Item(n));
}

inline int CColumnSet::Add(CColumn& oColumn)
{
	return CPtrArray::Add(&oColumn);
}

#endif //COLUMNSET_HPP
