/******************************************************************************
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

class CRowSet : protected TPtrArray<CRow>
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
	int   Count() const;
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

private:
	//
	// Disallow copy and assignment.
	//
	CRowSet(const CRowSet&);
	void operator=(const CRowSet&);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CRowSet::CRowSet()
{
}

inline CRowSet::~CRowSet()
{
	DeleteAll();
}

inline int CRowSet::Count() const
{
	return TPtrArray<CRow>::Size();
}

inline CRow& CRowSet::Row(int n) const
{
	return *(TPtrArray<CRow>::At(n));
}

inline CRow& CRowSet::operator[](int n) const
{
	return *(TPtrArray<CRow>::At(n));
}

inline int CRowSet::Add(CRow& oRow)
{
	return TPtrArray<CRow>::Add(&oRow);
}

inline void CRowSet::Remove(int nRow)
{
	TPtrArray<CRow>::Remove(nRow);
}

inline void CRowSet::Delete(int nRow)
{
	TPtrArray<CRow>::Delete(nRow);
}

inline void CRowSet::DeleteAll()
{
	TPtrArray<CRow>::DeleteAll();
}

inline bool CRowSet::Modified() const
{
	for (int i = 0; i < Count(); i++)
	{
		if (Row(i).Modified())
			return true;
	}

	return false;
}

#endif //ROWSET_HPP
