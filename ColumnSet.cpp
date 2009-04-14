/******************************************************************************
**
** MODULE:		COLUMNSET.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CColumnSet class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ColumnSet.hpp"

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

CColumnSet::CColumnSet()
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

CColumnSet::~CColumnSet()
{
	DeleteAll();
}

/******************************************************************************
** Method:		AllocSize()
**
** Description:	Calculates the number of bytes to allocate for a row.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

size_t CColumnSet::AllocSize() const
{
	size_t nSize = 0;

	for (size_t i = 0; i < Count(); ++i)
		nSize += Column(i).AllocSize();

	return nSize;
}

/******************************************************************************
** Method:		Delete()
**
** Description:	Delete the given column.
**
** Parameters:	n	The column to drop.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CColumnSet::Delete(size_t n)
{
	delete &Column(n);
	Remove(n);
}

/******************************************************************************
** Method:		DeleteAll()
**
** Description:	Delete all columns.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CColumnSet::DeleteAll()
{
	for (size_t i = 0; i < Count(); ++i)
		delete &Column(i);

	RemoveAll();
}

/******************************************************************************
** Method:		Find()
**
** Description:	Finds a column by name.
**
** Parameters:	pszName		The columns Name.
**
** Returns:		The columns index or -1 if not found.
**
*******************************************************************************
*/

size_t CColumnSet::Find(const tchar* pszName)
{
	ASSERT(pszName != NULL);

	for (size_t i = 0; i < Count(); ++i)
	{
		if (Column(i).Name() == pszName)
			return i;
	}

	return Core::npos;
}
