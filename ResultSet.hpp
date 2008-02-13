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

#if _MSC_VER > 1000
#pragma once
#endif

#include <Legacy/TArray.hpp>
#include "FwdDecls.hpp"
#include "Value.hpp"
#include "SortColumns.hpp"

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

	CResultSet& operator=(const CResultSet& oRHS);
	
	//
	// Accessors/Mutators.
	//
	size_t Count() const;
	CRow& Row(size_t n) const;
	CRow& operator[](size_t n) const;

	size_t Add(CRow& oRow);
	void  Truncate();

	//
	// Sorting methods.
	//
	void  OrderBy(const CSortColumns& oColumns);
	void  OrderBy(size_t nColumn, CSortColumns::Dir eDir);

	//
	// Aggregation methods.
	//
	CValue    Sum(size_t nColumn) const;
	CValue    Min(size_t nColumn) const;
	CValue    Max(size_t nColumn) const;
	CValueSet Distinct(size_t nColumn) const;
	CGroupSet GroupBy(size_t nColumn) const;

	//
	// Query methods.
	//
	CResultSet Select(const CWhere& oQuery) const;
	bool       Exists(const CWhere& oQuery) const;

	//
	// Debug methods.
	//
	void Dump(WCL::IOutputStream& rStream) const;

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

	//
	// Friends.
	//
	friend class CJoinedSet;

	//
	// qsort() helpers.
	//
	static const CSortColumns* g_pSortOrder;
	static int Compare(const void* ppRow1, const void* ppRow2);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline size_t CResultSet::Count() const
{
	return TPtrArray<CRow>::Size();
}

inline CRow& CResultSet::Row(size_t n) const
{
	ASSERT((n >= 0) && (n < Count()));

	return *(TPtrArray<CRow>::At(n));
}

inline CRow& CResultSet::operator[](size_t n) const
{
	ASSERT((n >= 0) && (n < Count()));

	return *(TPtrArray<CRow>::At(n));
}

inline size_t CResultSet::Add(CRow& oRow)
{
	return TPtrArray<CRow>::Add(&oRow);
}

inline void CResultSet::Truncate()
{
	TPtrArray<CRow>::RemoveAll();
}

inline void CResultSet::OrderBy(size_t nColumn, CSortColumns::Dir eDir)
{
	OrderBy(CSortColumns(nColumn, eDir));
}

#endif //RESULTSET_HPP
