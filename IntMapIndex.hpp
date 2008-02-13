/******************************************************************************
**
** MODULE:		INTMAPINDEX.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CIntMapIndex class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef INTMAPINDEX_HPP
#define INTMAPINDEX_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "UniqIndex.hpp"
#include <Legacy/IntPtrMap.hpp>
#include "Row.hpp"

/******************************************************************************
** 
** This class is used to index int columns using a MAP.
**
*******************************************************************************
*/

class CIntMapIndex : public CUniqIndex
{
public:
	//
	// Constructors/Destructor.
	//
	CIntMapIndex(CTable& oTable, size_t nColumn);
	virtual ~CIntMapIndex();
	
	//
	// Methods.
	//
	virtual size_t RowCount() const;
	virtual void AddRow(CRow& oRow);
	virtual void RemoveRow(CRow& oRow);
	virtual void Truncate();

	        CRow* FindRow(int nKey) const;
	virtual CRow* FindRow(const CValue& oValue) const;
	virtual CResultSet FindRows(const CValue& oValue) const;

	virtual void Capacity(size_t nRows);

protected:
	//
	// Members.
	//
	CIntPtrMap	m_oMap;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline size_t CIntMapIndex::RowCount() const
{
	return m_oMap.Count();
}

inline void CIntMapIndex::AddRow(CRow& oRow)
{
	ASSERT(FindRow(oRow[m_nColumn].GetValue()) == NULL);

	m_oMap.Add(oRow[m_nColumn], &oRow);
}

inline void CIntMapIndex::RemoveRow(CRow& oRow)
{
	m_oMap.Remove(oRow[m_nColumn]);
}

inline void CIntMapIndex::Truncate()
{
	m_oMap.RemoveAll();
}

inline CRow* CIntMapIndex::FindRow(int nKey) const
{
	return static_cast<CRow*>(m_oMap.Find(nKey));
}

inline CRow* CIntMapIndex::FindRow(const CValue& oValue) const
{
	ASSERT(oValue.m_eType == MDST_INT);

	return FindRow(oValue.m_iValue);
}

inline CResultSet CIntMapIndex::FindRows(const CValue& oValue) const
{
	ASSERT(oValue.m_eType == MDST_INT);

	return CResultSet(m_oTable, FindRow(oValue.m_iValue));
}

inline void CIntMapIndex::Capacity(size_t nRows)
{
	m_oMap.Reserve(nRows);
}

#endif //INTMAPINDEX_HPP
