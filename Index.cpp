/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		INDEX.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CIndex class definition.
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

CIndex::CIndex(CTable& oTable, int nColumn, bool bUnique)
	: m_oTable(oTable)
	, m_nColumn(nColumn)
	, m_bUnique(bUnique)
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

CIndex::~CIndex()
{
}
