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

/******************************************************************************
** 
** The class used to store the tables which belong to a database.
**
*******************************************************************************
*/

class CTableSet : public CPtrArray
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
	int     Count() const;
	CTable& Table(int n) const;
	CTable& operator[](int n) const;

	int  Add(CTable& oTable);
	void Remove(int nTable);
	void Delete(int nTable);

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

inline int CTableSet::Count() const
{
	return Size();
}

inline CTable& CTableSet::Table(int n) const
{
	return *((CTable*)CPtrArray::Item(n));
}

inline CTable& CTableSet::operator[](int n) const
{
	return *((CTable*)CPtrArray::Item(n));
}

inline int CTableSet::Add(CTable& oTable)
{
	return CPtrArray::Add(&oTable);
}

inline void CTableSet::Remove(int nTable)
{
	CPtrArray::Remove(nTable);
}

inline void CTableSet::Delete(int nTable)
{
	delete &Table(nTable);
	Remove(nTable);
}

#endif //TABLESET_HPP
