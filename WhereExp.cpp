/******************************************************************************
**
** MODULE:		WHEREEXP.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CWhereExp class definition.
**
*******************************************************************************
*/

#include "MDBL.hpp"

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

CWhereExp::CWhereExp(const CWhere& oLHSWhere, Op eOp, const CWhere& oRHSWhere)
	: m_pLHSWhere(oLHSWhere.Clone())
	, m_eOp(eOp)
	, m_pRHSWhere(oRHSWhere.Clone())
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

CWhereExp::CWhereExp(const CWhereExp& oSrc)
	: m_pLHSWhere(oSrc.m_pLHSWhere->Clone())
	, m_eOp(oSrc.m_eOp)
	, m_pRHSWhere(oSrc.m_pRHSWhere->Clone())
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

CWhereExp::~CWhereExp()
{
	delete m_pLHSWhere;
	delete m_pRHSWhere;
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

bool CWhereExp::Matches(const CRow& oRow) const
{
	bool bMatches = false;

	switch (m_eOp)
	{
		case AND:
			bMatches = (m_pLHSWhere->Matches(oRow) && m_pRHSWhere->Matches(oRow));
			break;

		case OR:
			bMatches = (m_pLHSWhere->Matches(oRow) || m_pRHSWhere->Matches(oRow));
			break;

		default:
			ASSERT_FALSE();
			break;
	};

	return bMatches;
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

CWhere* CWhereExp::Clone() const
{
	return new CWhereExp(*this);
}
