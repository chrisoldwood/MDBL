/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		ROWSET.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CRowSet class definition.
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

CRowSet::CRowSet()
{
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	Free all rows.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CRowSet::~CRowSet()
{
	DeleteAll();
}

/******************************************************************************
** Method:		Modified()
**
** Description:	Checks if any of the rows have been modified.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CRowSet::Modified() const
{
	for (int i = 0; i < Size(); i++)
	{
		if (Row(i).Modified())
			return true;
	}

	return false;
}

/******************************************************************************
** Method:		DeleteAll()
**
** Description:	Free all rows.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CRowSet::DeleteAll()
{
	for (int i = 0; i < Size(); i++)
		delete &Row(i);

	RemoveAll();
}
