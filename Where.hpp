/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		WHERE.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CWhere class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef WHERE_HPP
#define WHERE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "FwdDecls.hpp"

/******************************************************************************
** 
** This class is the base class for all WHERE clause types.
**
*******************************************************************************
*/

class CWhere
{
public:
	virtual ~CWhere();

	//
	// Methods
	//
	virtual bool Matches(const CRow& oRow) const = 0;

	virtual CWhere* Clone() const = 0;

protected:
	//
	// Make abstract.
	//
	CWhere();
	CWhere(const CWhere&);
	void operator=(const CWhere& oWhere);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CWhere::CWhere()
{
}

inline CWhere::~CWhere()
{
}

#endif //WHERE_HPP
