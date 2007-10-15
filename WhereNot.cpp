/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		WHERENOT.CPP
** COMPONENT:	Memory Database Library
** DESCRIPTION:	CWhereNot class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "WhereNot.hpp"

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CWhereNot::CWhereNot(const CWhere& oWhere)
	: m_pWhere(oWhere.Clone())
{
}

/******************************************************************************
** Method:		Copy Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CWhereNot::CWhereNot(const CWhereNot& oSrc)
	: m_pWhere(oSrc.m_pWhere->Clone())
{
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CWhereNot::~CWhereNot()
{
	delete m_pWhere;
}

/******************************************************************************
** Method:		Matches()
**
** Description:	Performs the comparisons.
**
** Parameters:	oRow	The row ro compare to.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CWhereNot::Matches(const CRow& oRow) const
{
	return !m_pWhere->Matches(oRow);
}

/******************************************************************************
** Method:		Clone()
**
** Description:	Create a copy of this object.
**
** Parameters:	None.
**
** Returns:		A copy of this.
**
*******************************************************************************
*/

CWhere* CWhereNot::Clone() const
{
	return new CWhereNot(*this);
}
