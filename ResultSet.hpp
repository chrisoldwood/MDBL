/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		RESULTSET.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CResultSet class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef RESULTSET_HPP
#define RESULTSET_HPP

/******************************************************************************
** 
** This is an array based class used to store the results of a table query.
**
*******************************************************************************
*/

class CResultSet : public CPtrArray
{
public:
	//
	// Constructors/Destructor.
	//
	CResultSet();
//	CResultSet(const CResultSet& oResultSet);
	CResultSet(const CRowSet& oRowSet);
	~CResultSet();
	
	//
	// Methods.
	//
	CRow& Row(int n) const;
	CRow& operator[](int n) const;

	int   Add(CRow& oRow);

private:
	//
	// Disallow copies/assignment for now.
	//
	void operator=(const CResultSet&);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CRow& CResultSet::Row(int n) const
{
	return *((CRow*) CPtrArray::Item(n));
}

inline CRow& CResultSet::operator[](int n) const
{
	return *((CRow*) CPtrArray::Item(n));
}

inline int CResultSet::Add(CRow& oRow)
{
	return CPtrArray::Add(&oRow);
}

#endif //RESULTSET_HPP
