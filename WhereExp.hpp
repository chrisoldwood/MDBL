/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		WHEREEXP.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CWhereExp class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef WHEREEXP_HPP
#define WHEREEXP_HPP

/******************************************************************************
** 
** This class is the class for composite WHERE clauses.
**
*******************************************************************************
*/

class CWhereExp : public CWhere
{
public:
	// Comparison ops.
	enum Op
	{
		AND,
		OR,
	};

	//
	// Constructors/Destructor.
	//
	CWhereExp(const CWhere& oLHSWhere, Op eOp, const CWhere& oRHSWhere);
	CWhereExp(const CWhereExp& oSrc);
	virtual ~CWhereExp();

	//
	// Methods
	//
	virtual bool Matches(const CRow& oRow) const;

	virtual CWhere* Clone() const;

private:
	//
	// Members.
	//
	const CWhere*	m_pLHSWhere;	// The left hand side expression.
	Op				m_eOp;			// The operator.
	const CWhere*	m_pRHSWhere;	// The right hand side expression.

	// Disallow assignment.
	void operator=(const CWhereExp& oWhere);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

/******************************************************************************
**
** Helper global operators for generating a WHERE caluse.
**
*******************************************************************************
*/

inline CWhereExp operator&&(const CWhere& oLHS, const CWhere& oRHS)
{
	return CWhereExp(oLHS, CWhereExp::AND, oRHS);
}

inline CWhereExp operator||(const CWhere& oLHS, const CWhere& oRHS)
{
	return CWhereExp(oLHS, CWhereExp::OR, oRHS);
}

#endif //WHEREEXP_HPP
