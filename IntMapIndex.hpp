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
#include "Row.hpp"
#include <map>

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
	//! The underlying collection type.
	typedef std::map<int, CRow*> IntRowMap;

	//
	// Members.
	//
	IntRowMap	m_oMap;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline size_t CIntMapIndex::RowCount() const
{
	return m_oMap.size();
}

inline void CIntMapIndex::AddRow(CRow& oRow)
{
	ASSERT(FindRow(oRow[m_nColumn].GetValue()) == NULL);

	m_oMap[oRow[m_nColumn]] = &oRow;
}

inline void CIntMapIndex::RemoveRow(CRow& oRow)
{
	m_oMap.erase(oRow[m_nColumn]);
}

inline void CIntMapIndex::Truncate()
{
	m_oMap.clear();
}

inline CRow* CIntMapIndex::FindRow(int nKey) const
{
	IntRowMap::const_iterator it = m_oMap.find(nKey);

	if (it == m_oMap.end())
		return NULL;

	return it->second;
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

inline void CIntMapIndex::Capacity(size_t /*nRows*/)
{
	// std::map<> does not optimise based on expected size.
}

#endif //INTMAPINDEX_HPP
