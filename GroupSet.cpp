/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		GROUPSET.CPP
** COMPONENT:	Memory Database Library
** DESCRIPTION:	CGroupSet class definition.
**
*******************************************************************************
*/

#include "mdbl.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

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
