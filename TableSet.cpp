/******************************************************************************
**
** MODULE:		TABLESET.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CTableSet class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "TableSet.hpp"
#include "Table.hpp"
#include <Core/Algorithm.hpp>

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

CTableSet::CTableSet()
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

CTableSet::~CTableSet()
{
}

void CTableSet::Delete(size_t nTable)
{
	Core::deleteAt(*this, nTable);
}
