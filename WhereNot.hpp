/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		WHERENOT.HPP
** COMPONENT:	Memory Database Library
** DESCRIPTION:	The CWhereNot class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef WHERENOT_HPP
#define WHERENOT_HPP

/******************************************************************************
** 
** This class is used for negating other WHERE clauses.
**
*******************************************************************************
*/

class CWhereNot : public CWhere
{
public:
	//
	// Constructors/Destructor.
	//
	CWhereNot(const CWhere& oWhere);
	CWhereNot(const CWhereNot& oSrc);
	virtual ~CWhereNot();
	
	//
	// Methods
	//
	virtual bool Matches(const CRow& oRow) const;

	virtual CWhere* Clone() const;

private:
	//
	// Members.
	//
	const CWhere* m_pWhere;		// The expression to negate.

	// Disallow assignment.
	void operator=(const CWhereNot& oWhere);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // WHERENOT_HPP
