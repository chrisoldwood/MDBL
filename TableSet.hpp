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
	return *(TPtrArray<CTable>::At(n));
}

inline CTable& CTableSet::operator[](int n) const
{
	return *(TPtrArray<CTable>::At(n));
}

inline int CTableSet::Add(CTable& oTable)
{
	return TPtrArray<CTable>::Add(&oTable);
}

inline void CTableSet::Remove(int nTable)
{
	TPtrArray<CTable>::Remove(nTable);
}

inline void CTableSet::Delete(int nTable)
{
	TPtrArray<CTable>::Delete(nTable);
}

#endif //TABLESET_HPP
