/******************************************************************************
**
** MODULE:		STRMAPINDEX.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CStrMapIndex class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef STRMAPINDEX_HPP
#define STRMAPINDEX_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "UniqIndex.hpp"
#include <Legacy/StrPtrMap.hpp>
#include "Row.hpp"

/******************************************************************************
** 
** This class is used to index String columns using a MAP.
**
*******************************************************************************
*/

class CStrMapIndex : public CUniqIndex
{
public:
	//
	// Constructors/Destructor.
	//
	CStrMapIndex(CTable& oTable, size_t nColumn);
	virtual ~CStrMapIndex();
	
	//
	// Methods.
	//
	virtual size_t RowCount() const;
	virtual void AddRow(CRow& oRow);
	virtual void RemoveRow(CRow& oRow);
	virtual void Truncate();

	        CRow* FindRow(const tchar* strKey) const;
	virtual CRow* FindRow(const CValue& oValue) const;
	virtual CResultSet FindRows(const CValue& oValue) const;

	virtual void Capacity(size_t nRows);

protected:
	//
	// Members.
	//
	CStrPtrMap	m_oMap;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline size_t CStrMapIndex::RowCount() const
{
	return m_oMap.Count();
}

inline void CStrMapIndex::AddRow(CRow& oRow)
{
	ASSERT(FindRow(oRow[m_nColumn].GetValue()) == NULL);

	m_oMap.Add(oRow[m_nColumn].GetString(), &oRow);
}

inline void CStrMapIndex::RemoveRow(CRow& oRow)
{
	m_oMap.Remove(oRow[m_nColumn].GetString());
}

inline void CStrMapIndex::Truncate()
{
	m_oMap.RemoveAll();
}

inline CRow* CStrMapIndex::FindRow(const tchar* strKey) const
{
	return static_cast<CRow*>(m_oMap.Find(strKey));
}

inline CRow* CStrMapIndex::FindRow(const CValue& oValue) const
{
	ASSERT(oValue.m_eType == MDST_STRING);

	return FindRow(oValue.m_sValue);
}

inline CResultSet CStrMapIndex::FindRows(const CValue& oValue) const
{
	ASSERT(oValue.m_eType == MDST_STRING);

	return CResultSet(m_oTable, FindRow(oValue.m_sValue));
}

inline void CStrMapIndex::Capacity(size_t nRows)
{
	m_oMap.Reserve(nRows);
}

#endif //STRMAPINDEX_HPP
