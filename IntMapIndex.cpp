/******************************************************************************
**
** MODULE:		INTMAPINDEX.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CIntMapIndex class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "IntMapIndex.hpp"
#include "Table.hpp"

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

CIntMapIndex::CIntMapIndex(CTable& oTable, size_t nColumn)
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
