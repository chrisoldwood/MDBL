/******************************************************************************
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
** Parameters:	nTable		The initial table.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CJoin::CJoin(int nTable)
{
	CPtrArray::Add(new CJoinTable(nTable, -1, -1, -1));
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
	for (int i = 0; i < Count(); i++)
		delete &Join(i);

	RemoveAll();
}

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	nTable		The table to join on.
**				nColumn		The column to join on.
**				pWhere		Additional where clause for this table.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CJoinTable::CJoinTable(int nTable, int nLHSColumn, int nType, int nRHSColumn)
	: m_nTable(nTable)
	, m_nLHSColumn(nLHSColumn)
	, m_nType(nType)
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

CJoinTable::~CJoinTable()
{
}
