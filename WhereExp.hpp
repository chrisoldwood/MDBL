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
** This class is the base class for composite WHERE clauses.
**
*******************************************************************************
*/

class CWhereExp : public CWhere
{
public:
	//
	// Methods
	//
	virtual bool Matches(const CRow& oRow) const = 0;

protected:
	//
	// Constructors/Destructor.
	//
	CWhereExp(const CWhere& oLHSWhere, const CWhere& oRHSWhere);
	virtual ~CWhereExp();

	//
	// Members.
	//
	const CWhere* m_pLHSWhere;	// The left hand side expression.
	const CWhere* m_pRHSWhere;	// The right hand side expression.

private:
	//
	// Disallow copies.
	//
//	CWhereExp(const CWhereExp& oWhere);
	void operator=(const CWhereExp& oWhere);
};

/******************************************************************************
** 
** The CWhereExp derived classes to implement the expression.
**
*******************************************************************************
*/

class CWhereAnd : public CWhereExp
{
public:
	//
	// Constructors/Destructor.
	//
	CWhereAnd(const CWhere& oLHSWhere, const CWhere& oRHSWhere);

	//
	// Methods
	//
	virtual bool Matches(const CRow& oRow) const;
};

class CWhereOr : public CWhereExp
{
public:
	//
	// Constructors/Destructor.
	//
	CWhereOr(const CWhere& oLHSWhere, const CWhere& oRHSWhere);

	//
	// Methods
	//
	virtual bool Matches(const CRow& oRow) const;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CWhereExp::CWhereExp(const CWhere& oLHSWhere, const CWhere& oRHSWhere)
	: m_pLHSWhere(&oLHSWhere)
	, m_pRHSWhere(&oRHSWhere)
{
}

inline CWhereExp::~CWhereExp()
{
}

inline CWhereAnd::CWhereAnd(const CWhere& oLHSWhere, const CWhere& oRHSWhere)
	: CWhereExp(oLHSWhere, oRHSWhere)
{
}

inline bool CWhereAnd::Matches(const CRow& oRow) const
{
	if (m_pLHSWhere->Matches(oRow) == false)
		return false;
	
	return m_pRHSWhere->Matches(oRow);
}

inline CWhereOr::CWhereOr(const CWhere& oLHSWhere, const CWhere& oRHSWhere)
	: CWhereExp(oLHSWhere, oRHSWhere)
{
}

inline bool CWhereOr::Matches(const CRow& oRow) const
{
	if (m_pLHSWhere->Matches(oRow) == true)
		return true;

	return m_pRHSWhere->Matches(oRow);
}

/******************************************************************************
**
** Helper fundtions for generating a WHERE caluse.
**
*******************************************************************************
*/

inline CWhereAnd operator&(const CWhere& oLHS, const CWhere& oRHS)
{
	return CWhereAnd(oLHS, oRHS);
}

inline CWhereOr operator|(const CWhere& oLHS, const CWhere& oRHS)
{
	return CWhereOr(oLHS, oRHS);
}

#endif //WHEREEXP_HPP
