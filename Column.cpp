/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		COLUMN.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CColumn class definition.
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

CColumn::CColumn(CTable& oTable, const char* pszName, COLTYPE eType, int nLength, int nAllocSize, int nFlags)
	: m_oTable(oTable)
	, m_strName(pszName)
	, m_eColType(eType)
	, m_eStgType(ColToStgType(eType))
	, m_nLength(nLength)
	, m_nAllocSize((nAllocSize + 3) & ~3)
	, m_nFlags(nFlags)
	, m_pFKTable(NULL)
	, m_nFKColumn(-1)
	, m_pIndex(NULL)
{
	ASSERT(pszName      != NULL);
	ASSERT(m_eStgType   != MDST_NULL);
	ASSERT(m_nAllocSize >  0);
	ASSERT(!(Nullable() && Unique()));
	ASSERT((nFlags & FOREIGN_KEY) == 0);
}

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

CColumn::CColumn(CTable& oTable, const char* pszName, int nFlags, CTable& oFKTable, int nFKColumn, const CColumn& oFKColumn)
	: m_oTable(oTable)
	, m_strName(pszName)
	, m_eColType(oFKColumn.ColType())
	, m_eStgType(ColToStgType(oFKColumn.ColType()))
	, m_nLength(oFKColumn.Length())
	, m_nAllocSize(oFKColumn.AllocSize())
	, m_nFlags(nFlags)
	, m_pFKTable(&oFKTable)
	, m_nFKColumn(nFKColumn)
	, m_pIndex(NULL)
{
	ASSERT(pszName      != NULL);
	ASSERT(m_eStgType   != MDST_NULL);
	ASSERT(m_nAllocSize >  0);
	ASSERT(!(Nullable() && Unique()));
	ASSERT((nFlags & FOREIGN_KEY) != 0);

	// If Identity, convert to int.
	if (m_eColType == MDCT_IDENTITY)
		m_eColType = MDCT_INT;
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

CColumn::~CColumn()
{
	delete m_pIndex;
}

/******************************************************************************
** Method:		ColToStgType()
**
** Description:	Finds the storage type required for the given column type.
**
** Parameters:	None.
**
** Returns:		The storage type.
**
*******************************************************************************
*/

STGTYPE CColumn::ColToStgType(COLTYPE eType)
{
	switch (eType)
	{
		case MDCT_INT:			return MDST_INT;
		case MDCT_DOUBLE:		return MDST_DOUBLE;
		case MDCT_CHAR:			return MDST_CHAR;
		case MDCT_FXDSTR:		return MDST_STRING;
		case MDCT_VARSTR:		return MDST_STRING;
		case MDCT_BOOL:			return MDST_BOOL;
		case MDCT_IDENTITY:		return MDST_INT;
		case MDCT_DATETIME:		return MDST_TIME_T;
		case MDCT_TIMESTAMP:	return MDST_TIMESTAMP;
	}

	ASSERT(false);

	return MDST_NULL;
}

/******************************************************************************
** Method:		Index()
**
** Description:	Sets a new index on the column, and deletes the old one.
**
** Parameters:	pIndex	The index to use.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CColumn::Index(CIndex* pIndex)
{
	delete m_pIndex;
	m_pIndex = pIndex;
}
