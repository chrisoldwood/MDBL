/******************************************************************************
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
** Description:	Constructs a result set with possibly only a single row.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CResultSet::CResultSet(CRow* pRow)
{
	if (pRow != NULL)
		Add(*pRow);
}

/******************************************************************************
** Method:		Copy constructor.
**
** Description:	Copies the existing result set.
**
** Parameters:	oResultSet
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CResultSet::CResultSet(const CResultSet& oResultSet)
{
	Reserve(oResultSet.Count());

	for (int i = 0; i < oResultSet.Count(); i++)
		Add(oResultSet[i]);
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
	Reserve(oRowSet.Count());

	for (int i = 0; i < oRowSet.Count(); i++)
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
