/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		JOIN.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CJoin class definition.
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

CJoin::CJoin(int nLHSTable, int nLHSColumn, int nRHSTable, int nRHSColumn)
	: m_nLHSTable(nLHSTable)
	, m_nLHSColumn(nLHSColumn)
	, m_nRHSTable(nRHSTable)
	, m_nRHSColumn(nRHSColumn)
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

CJoin::~CJoin()
{
}
