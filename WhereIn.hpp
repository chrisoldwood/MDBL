/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		WHEREIN.HPP
** COMPONENT:	Memory Database Library
** DESCRIPTION:	The CWhereIn class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef WHEREIN_HPP
#define WHEREIN_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Where.hpp"
#include "ValueSet.hpp"

/******************************************************************************
** 
** This class is used for WHERE clause comparisons of multiple value.
**
*******************************************************************************
*/

class CWhereIn : public CWhere
{
public:
	//
	// Constructors/Destructor.
	//
	CWhereIn(size_t nColumn, const CValueSet& oValueSet);
	CWhereIn(const CWhereIn& oSrc);
	virtual ~CWhereIn();
	
	//
	// Methods
	//
	virtual bool Matches(const CRow& oRow) const;

	virtual CWhere* Clone() const;

private:
	//
	// Members.
	//
	size_t		m_nColumn;		// The column to check.
	CValueSet	m_oValueSet;	// The values to match.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // WHEREIN_HPP
