/******************************************************************************
**
** MODULE:		COLUMN.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CColumn class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "Column.hpp"
#include "Index.hpp"
#include <algorithm>

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

CColumn::CColumn(CTable& oTable, const tchar* pszName, COLTYPE eType, size_t nLength, size_t nAllocSize, uint nFlags)
	: m_oTable(oTable)
	, m_strName(pszName)
	, m_eColType(eType)
	, m_eStgType(ColToStgType(eType))
	, m_nLength(nLength)
	, m_nAllocSize((nAllocSize + 3) & ~3)
	, m_nFlags(nFlags)
	, m_pFKTable(NULL)
	, m_nFKColumn(0)
	, m_pIndex(NULL)
{
	ASSERT(pszName      != NULL);
	ASSERT(m_eStgType   != MDST_NULL);
	ASSERT(!(Nullable() && Unique()));
	ASSERT(!(Nullable() && PrimaryKey()));
	ASSERT(!ForeignKey());
	ASSERT(!((m_eStgType == MDST_POINTER) && (!Transient())));
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

CColumn::CColumn(CTable& oTable, const tchar* pszName, CTable& oFKTable, size_t nFKColumn, const CColumn& oFKColumn, uint nFlags)
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
	ASSERT(ForeignKey());
	ASSERT(m_eStgType != MDST_POINTER);

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
		case MDCT_INT64:		return MDST_INT64;
		case MDCT_DOUBLE:		return MDST_DOUBLE;
		case MDCT_CHAR:			return MDST_CHAR;
		case MDCT_FXDSTR:		return MDST_STRING;
		case MDCT_VARSTR:		return MDST_STRING;
		case MDCT_BOOL:			return MDST_BOOL;
		case MDCT_IDENTITY:		return MDST_INT;
		case MDCT_DATETIME:		return MDST_INT64;
		case MDCT_DATE:			return MDST_INT64;
		case MDCT_TIME:			return MDST_INT64;
		case MDCT_TIMESTAMP:	return MDST_TIMESTAMP;
		case MDCT_VOIDPTR:		return MDST_POINTER;
		case MDCT_ROWPTR:		return MDST_POINTER;
		case MDCT_ROWSETPTR:	return MDST_POINTER;

		default:				ASSERT_FALSE();	break;
	}

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

/******************************************************************************
** Method:		DisplayWidth()
**
** Description:	Gets the char width of the column for display.
**
** Parameters:	bDebug		Get width for debug format strings?
**
** Returns:		The width.
**
*******************************************************************************
*/

size_t CColumn::DisplayWidth(bool bDebug) const
{
	// Normal format?
	if (!bDebug)
	{
		switch (m_eColType)
		{
			case MDCT_INT:			return 10;
			case MDCT_INT64:		return 19;
			case MDCT_DOUBLE:		return 15;
			case MDCT_CHAR:			return 1;
			case MDCT_FXDSTR:		return m_nLength;
			case MDCT_VARSTR:		return m_nLength;
			case MDCT_BOOL:			return 1;
			case MDCT_IDENTITY:		return 10;
			case MDCT_DATETIME:		return 17;
			case MDCT_DATE:			return 8;
			case MDCT_TIME:			return 8;
			case MDCT_TIMESTAMP:	return 17;
			case MDCT_VOIDPTR:		return 8;
			case MDCT_ROWPTR:		return 8;
			case MDCT_ROWSETPTR:	return 8;

			default:				ASSERT_FALSE();	break;
		}
	}
	// Debug format?
	else
	{
		size_t nMin = (Nullable()) ? 5 : 0;

		switch (m_eStgType)
		{
			case MDST_INT:			return std::max<size_t>(nMin, 10u);
			case MDST_INT64:		return std::max<size_t>(nMin, 19u);
			case MDST_DOUBLE:		return std::max<size_t>(nMin, 15u);
			case MDST_CHAR:			return std::max<size_t>(nMin, 1u);
			case MDST_STRING:		return std::max<size_t>(nMin, m_nLength);
			case MDST_BOOL:			return std::max<size_t>(nMin, 1u);
			case MDST_TIMESTAMP:	return std::max<size_t>(nMin, 17u);
			case MDST_POINTER:		return std::max<size_t>(nMin, 10u);

			case MDST_NULL:
			default:				ASSERT_FALSE();	break;
		}
	}

	return 0;
}
