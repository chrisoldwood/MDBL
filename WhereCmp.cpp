/******************************************************************************
**
** MODULE:		WHERECMP.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CWhereCmp class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "WhereCmp.hpp"
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

CWhereCmp::CWhereCmp(size_t nColumn, Op eOp, const CValue& oValue)
	: m_nColumn(nColumn)
	, m_eOp(eOp)
	, m_oValue(oValue)
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

CWhereCmp::CWhereCmp(const CWhereCmp& oSrc)
	: m_nColumn(oSrc.m_nColumn)
	, m_eOp(oSrc.m_eOp)
	, m_oValue(oSrc.m_oValue)
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

CWhereCmp::~CWhereCmp()
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

bool CWhereCmp::Matches(const CRow& oRow) const
{
	bool bMatches = false;
	int  nResult  = oRow[m_nColumn].Compare(m_oValue);

	switch (m_eOp)
	{
		case EQUALS:
			bMatches = (nResult == 0);
			break;

		case NOT_EQUALS:
			bMatches = (nResult != 0);
			break;

		case GREATER:
			bMatches = (nResult > 0);
			break;

		case LESS:
			bMatches = (nResult < 0);
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

CWhere* CWhereCmp::Clone() const
{
	return new CWhereCmp(*this);
}
