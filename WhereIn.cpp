/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		WHEREIN.CPP
** COMPONENT:	Memory Database Library
** DESCRIPTION:	CWhereIn class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "WhereIn.hpp"
#include "Row.hpp"

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

CWhereIn::CWhereIn(size_t nColumn, const CValueSet& oValueSet)
	: m_nColumn(nColumn)
	, m_oValueSet(oValueSet)
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

CWhereIn::CWhereIn(const CWhereIn& oSrc)
	: m_nColumn(oSrc.m_nColumn)
	, m_oValueSet(oSrc.m_oValueSet)
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

CWhereIn::~CWhereIn()
{
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

bool CWhereIn::Matches(const CRow& oRow) const
{
	// For all values...
	for (size_t i = 0; i < m_oValueSet.Count(); ++i)
	{
		if (oRow[m_nColumn] == m_oValueSet[i])
			return true;
	}

	return false;
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

CWhere* CWhereIn::Clone() const
{
	return new CWhereIn(*this);
}
