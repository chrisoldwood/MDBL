/******************************************************************************
**
** MODULE:		JOINEDSET.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CJoinedSet class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "JoinedSet.hpp"

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

CJoinedSet::CJoinedSet(size_t nTables, CTable* apTables[])
	: m_nTables(nTables)
	, m_pResSets(new CResultSet[nTables])
{
	ASSERT(m_nTables > 0);

	// Setup table pointers in the result sets.
	for (size_t i = 0; i < m_nTables; ++i)
		m_pResSets[i].m_pTable = apTables[i];
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

	pRHS->m_pResSets = nullptr;
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
