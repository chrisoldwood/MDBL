/******************************************************************************
**
** MODULE:		INTMAPINDEX.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CIntMapIndex class definition.
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

CIntMapIndex::CIntMapIndex(CTable& oTable, int nColumn)
	: CUniqIndex(oTable, nColumn)
{
	ASSERT(m_oTable.Column(m_nColumn).Unique());
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

CIntMapIndex::~CIntMapIndex()
{
}
