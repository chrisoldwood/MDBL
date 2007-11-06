/******************************************************************************
**
** MODULE:		UNIQINDEX.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CUniqIndex class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef UNIQINDEX_HPP
#define UNIQINDEX_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Index.hpp"

/******************************************************************************
** 
** This is the base class for all unique index classes.
**
*******************************************************************************
*/

class CUniqIndex : public CIndex
{
public:
	//
	// Methods.
	//
	virtual CRow* FindRow(const CValue& oValue) const = 0;

protected:
	//
	// Constructors/Destructor.
	//
	CUniqIndex(CTable& oTable, int nColumn);
	virtual ~CUniqIndex();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CUniqIndex::CUniqIndex(CTable& oTable, int nColumn)
	: CIndex(oTable, nColumn)
{
}

inline CUniqIndex::~CUniqIndex()
{
}

#endif //UNIQINDEX_HPP
