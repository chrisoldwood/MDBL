/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		RESULTSET.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CResultSet class definition.
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

CResultSet::CResultSet()
{
}

/******************************************************************************
** Method:		Constructor.
**
** Description:	Constructs the result set as a copy of the RowSet.
**
** Parameters:	oRowSet		The RowSet to copy.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CResultSet::CResultSet(const CRowSet& oRowSet)
{
	Reserve(oRowSet.Size());

	for (int i = 0; i < oRowSet.Size(); i++)
		Add(oRowSet[i]);
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

CResultSet::~CResultSet()
{
}
