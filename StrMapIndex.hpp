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
#include "Row.hpp"
#include <map>

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
	//! The underlying collection type.
	typedef std::map<CString, CRow*> StrRowMap;

	//
	// Members.
	//
	StrRowMap	m_oMap;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline size_t CStrMapIndex::RowCount() const
{
	return m_oMap.size();
}

inline void CStrMapIndex::AddRow(CRow& oRow)
{
	ASSERT(FindRow(oRow[m_nColumn].ToValue()) == NULL);

	m_oMap[oRow[m_nColumn].GetString()] = &oRow;
}

inline void CStrMapIndex::RemoveRow(CRow& oRow)
{
	m_oMap.erase(oRow[m_nColumn].GetString());
}

inline void CStrMapIndex::Truncate()
{
	m_oMap.clear();
}

inline CRow* CStrMapIndex::FindRow(const tchar* strKey) const
{
	StrRowMap::const_iterator it = m_oMap.find(strKey);

	if (it == m_oMap.end())
		return NULL;

	return it->second;
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

inline void CStrMapIndex::Capacity(size_t /*nRows*/)
{
	// std::map<> does not optimise based on expected size.
}

#endif //STRMAPINDEX_HPP
