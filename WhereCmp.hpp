/******************************************************************************
**
** MODULE:		WHERECMP.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CWhereCmp class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef WHERECMP_HPP
#define WHERECMP_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Where.hpp"
#include "Value.hpp"

/******************************************************************************
** 
** This class is used for WHERE clause comparisons of values.
**
*******************************************************************************
*/

class CWhereCmp : public CWhere
{
public:
	// Comparison ops.
	enum Op
	{
		EQUALS,
		NOT_EQUALS,
		GREATER,
		LESS,
	};

	//
	// Constructors/Destructor.
	//
	CWhereCmp(int nColumn, Op eOp, const CValue& oValue);
	CWhereCmp(const CWhereCmp& oSrc);
	virtual ~CWhereCmp();

	//
	// Methods
	//
	virtual bool Matches(const CRow& oRow) const;

	virtual CWhere* Clone() const;

private:
	//
	// Members.
	//
	int		m_nColumn;	// The column to check.
	Op		m_eOp;		// The operator.
	CValue	m_oValue;	// The value to match.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //WHERECMP_HPP
