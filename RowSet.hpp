/******************************************************************************
** (C) Chris Oldwood
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

/******************************************************************************
** 
** The class used to store the rows which belong to a table.
**
*******************************************************************************
*/

class CRowSet : public CPtrArray
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
	CRow& Row(int n) const;
	CRow& operator[](int n) const;

	int   Add(CRow& oRow);
	void  Remove(int nRow);
	void  Delete(int nRow);
	void  DeleteAll();

	bool  Modified() const;

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

inline CRow& CRowSet::Row(int n) const
{
	return *((CRow*) CPtrArray::Item(n));
}

inline CRow& CRowSet::operator[](int n) const
{
	return *((CRow*) CPtrArray::Item(n));
}

inline int CRowSet::Add(CRow& oRow)
{
	return CPtrArray::Add(&oRow);
}

inline void CRowSet::Remove(int nRow)
{
	CPtrArray::Remove(nRow);
}

inline void CRowSet::Delete(int nRow)
{
	delete &Row(nRow);
	CPtrArray::Remove(nRow);
}

#endif //ROWSET_HPP
