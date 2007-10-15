/******************************************************************************
**
** MODULE:		STRMAPINDEX.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CStrMapIndex class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "StrMapIndex.hpp"
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

CStrMapIndex::CStrMapIndex(CTable& oTable, int nColumn)
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

CStrMapIndex::~CStrMapIndex()
{
}
