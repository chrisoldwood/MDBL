/******************************************************************************
**
** MODULE:		RESULTSET.H
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

class CResultSet : protected TPtrArray<CRow>
{
public:
	//
	// Constructors/Destructor.
	//
	CResultSet(const CTable& oTable);
	CResultSet(const CTable& oTable, CRow* pRow);
	CResultSet(const CResultSet& oResultSet);
	CResultSet(const CTable& oTable, const CRowSet& oRowSet);
	~CResultSet();
	
	//
	// Methods.
	//
	int   Count() const;
	CRow& Row(int n) const;
	CRow& operator[](int n) const;

	int   Add(CRow& oRow);

	void  OrderBy(const CSortColumns& oColumns);
	void  OrderBy(int nColumn, CSortColumns::Dir eDir);

	CValue    Sum(int nColumn) const;
	CValue    Min(int nColumn) const;
	CValue    Max(int nColumn) const;
	CValueSet Distinct(int nColumn) const;

	//
	// Debug methods.
	//
	void Dump(CStream& rStream) const;

protected:
	//
	// Members.
	//
	const CTable*	m_pTable;	// The table the rows belong to.

private:
	//
	// Disallow array constructs except via friends and assignment.
	//
	CResultSet();
	void operator=(const CResultSet&);

	//
	// Friends.
	//
	friend class CJoinedSet;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline int CResultSet::Count() const
{
	return TPtrArray<CRow>::Size();
}

inline CRow& CResultSet::Row(int n) const
{
	ASSERT((n >= 0) && (n < Count()));

	return *(TPtrArray<CRow>::At(n));
}

inline CRow& CResultSet::operator[](int n) const
{
	ASSERT((n >= 0) && (n < Count()));

	return *(TPtrArray<CRow>::At(n));
}

inline int CResultSet::Add(CRow& oRow)
{
	return TPtrArray<CRow>::Add(&oRow);
}

inline void CResultSet::OrderBy(int nColumn, CSortColumns::Dir eDir)
{
	OrderBy(CSortColumns(nColumn, eDir));
}

#endif //RESULTSET_HPP
