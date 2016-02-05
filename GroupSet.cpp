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
	: m_oResSets()
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
	: m_oResSets()
{
	m_oResSets.reserve(oSet.Count());

	for (size_t i = 0; i < oSet.Count(); ++i)
	{
		m_oResSets.push_back(new CResultSet(oSet[i]));
	}
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
	for (size_t i = 0; i != m_oResSets.size(); ++i)
		delete m_oResSets[i];

	m_oResSets.clear();
}
