/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		INTINDEX.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CIntIndex class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef INTINDEX_HPP
#define INTINDEX_HPP

/******************************************************************************
** 
** This class is used to index int columns.
**
*******************************************************************************
*/

class CIntIndex : public CIndex
{
public:
	//
	// Constructors/Destructor.
	//
	CIntIndex(CTable& oTable, int nColumn, bool bUnique);
	virtual ~CIntIndex();
	
	//
	// Methods.
	//
	virtual void AddRow(CRow& oRow);
	virtual void RemoveRow(CRow& oRow);
	virtual void Truncate();

	virtual CRow* FindRow(const CValue& oValue) const;
	        CRow* FindRow(int nKey) const;

protected:
	//
	// Members.
	//
	CIntPtrMap	m_oMap;

	// Default size of the map.
	enum { DEF_MAP_SIZE = 17 };
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline void CIntIndex::AddRow(CRow& oRow)
{
	m_oMap.Add(oRow[m_nColumn], &oRow);
}

inline void CIntIndex::RemoveRow(CRow& oRow)
{
	m_oMap.Remove(oRow[m_nColumn]);
}

inline void CIntIndex::Truncate()
{
	m_oMap.RemoveAll();
}

inline CRow* CIntIndex::FindRow(const CValue& oValue) const
{
	ASSERT(oValue.m_eType == MDST_INT);

	return FindRow(oValue.m_iValue);
}

inline CRow* CIntIndex::FindRow(int nKey) const
{
	return (CRow*) m_oMap.Find(nKey);
}

#endif //INTINDEX_HPP
