/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		GROUPSET.CPP
** COMPONENT:	Memory Database Library
** DESCRIPTION:	CGroupSet class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "GroupSet.hpp"

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

CGroupSet::CGroupSet()
{
}

/******************************************************************************
** Method:		Copy Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CGroupSet::CGroupSet(const CGroupSet& oSet)
{
	m_oResSets.DeepCopy(oSet.m_oResSets);
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

CGroupSet::~CGroupSet()
{
	m_oResSets.DeleteAll();
}
