/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		INTINDEX.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CIntIndex class definition.
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

CIntIndex::CIntIndex(CTable& oTable, int nColumn, bool bUnique)
	: CIndex(oTable, nColumn, bUnique)
	, m_oMap(DEF_MAP_SIZE)
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

CIntIndex::~CIntIndex()
{
}
