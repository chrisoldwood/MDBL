/******************************************************************************
**
** MODULE:		JOINEDSET.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CJoinedSet class definition.
**
*******************************************************************************
*/

#include "MDBL.hpp"

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	nTables		The number of tables in the join.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CJoinedSet::CJoinedSet(int nTables)
	: m_nTables(nTables)
	, m_pResSets(new CResultSet[nTables])
{
	ASSERT(m_nTables > 0);
}

/******************************************************************************
** Method:		Copy constructor.
**
** Description:	Transfers ownership of the data.
**
** Parameters:	oJS
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CJoinedSet::CJoinedSet(const CJoinedSet& oJoinedSet)
	: m_nTables(oJoinedSet.m_nTables)
	, m_pResSets(oJoinedSet.m_pResSets)
{
	CJoinedSet* pRHS = const_cast<CJoinedSet*>(&oJoinedSet);

	pRHS->m_pResSets = NULL;
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

CJoinedSet::~CJoinedSet()
{
	delete[] m_pResSets;
}
