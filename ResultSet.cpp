/******************************************************************************
**
** MODULE:		RESULTSET.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CResultSet class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ResultSet.hpp"
#include "Row.hpp"
#include "RowSet.hpp"
#include "Table.hpp"
#include "ValueSet.hpp"
#include "GroupSet.hpp"
#include "Where.hpp"
#include <WCL/IInputStream.hpp>
#include <WCL/IOutputStream.hpp>
#include <malloc.h>

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

CResultSet::CResultSet()
	: m_pTable(nullptr)
{
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

CResultSet::CResultSet(const CTable& oTable)
	: m_pTable(&oTable)
{
}

/******************************************************************************
** Method:		Constructor.
**
** Description:	Constructs a result set with possibly only a single row.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CResultSet::CResultSet(const CTable& oTable, CRow* pRow)
	: m_pTable(&oTable)
{
	if (pRow != nullptr)
		Add(*pRow);
}

/******************************************************************************
** Method:		Copy constructor.
**
** Description:	Copies the existing result set.
**
** Parameters:	oResultSet
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CResultSet::CResultSet(const CResultSet& oResultSet)
    : Collection()
	, m_pTable(oResultSet.m_pTable)
{
	// Copy rows.
	Collection::reserve(oResultSet.Count());

	for (size_t i = 0; i < oResultSet.Count(); ++i)
		Collection::push_back(&oResultSet[i]);
}

/******************************************************************************
** Method:		Constructor.
**
** Description:	Constructs the result set as a copy of the RowSet.
**
** Parameters:	oRowSet		The RowSet to copy.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CResultSet::CResultSet(const CTable& oTable, const CRowSet& oRowSet)
	: m_pTable(&oTable)
{
	// Copy rows.
	Collection::reserve(oRowSet.Count());

	for (size_t i = 0; i < oRowSet.Count(); ++i)
		Collection::push_back(&oRowSet[i]);
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

CResultSet::~CResultSet()
{
}

/******************************************************************************
** Method:		Assignment operator.
**
** Description:	.
**
** Parameters:	oRHS	The object to copy.
**
** Returns:		itself.
**
*******************************************************************************
*/

CResultSet& CResultSet::operator=(const CResultSet& oRHS)
{
	ASSERT(this != &oRHS);

	// Clear state.
	Collection::clear();

	// Copy members.
	m_pTable = oRHS.m_pTable;

	// Copy rows.
	Collection::reserve(oRHS.Count());

	for (size_t i = 0; i < oRHS.Count(); ++i)
		Collection::push_back(&oRHS[i]);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//! The comparison functor used to compare two rows when sorting.

class Comparator
{
public:
	Comparator(const CSortColumns& sortOrder)
		: m_sortOrder(sortOrder)
	{ }

	//! Query if the lhs < rhs.
	bool operator()(const CRow* row1, const CRow* row2)
	{
		return (compare(row1, row2) < 0);
	}

private:
	const CSortColumns& m_sortOrder;

	//! Provide a full comparsion of the two rows.
	int compare(const CRow* row1, const CRow* row2);
};

////////////////////////////////////////////////////////////////////////////////
//! Provide a full comparsion of the two rows.

int Comparator::compare(const CRow* row1, const CRow* row2)
{
	for (size_t k = 0; k < m_sortOrder.Count(); ++k)
	{
		size_t column = m_sortOrder.Column(k);
		int    direction = m_sortOrder.Direction(k);

		int result = row1->Field(column).Compare(row2->Field(column));

		if (result != 0)
			return (direction == CSortColumns::ASC) ? result : -result;
	}

	return 0;
}

/******************************************************************************
** Method:		OrderBy()
**
** Description:	Sort the result by the columns specified.
**				NB: This uses qsort() which needs a C style callback function.
**					Consequently we need to use a static member to hold the
**					sorting definition so this method is not thread safe.
**
** Parameters:	oColumns	The columns and orders to sort by.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CResultSet::OrderBy(const CSortColumns& oColumns)
{
	std::sort(begin(), end(), Comparator(oColumns));
}

/******************************************************************************
** Methods:		Sum() Min() Max()
**
** Description:	Calculates the Sum/Finds the Min or Max values of a column.
**
** Parameters:	nColumn		The index of the column to sum/search.
**
** Returns:		The result.
**
*******************************************************************************
*/

CValue CResultSet::Sum(size_t nColumn) const
{
	const CColumn& oColumn = m_pTable->Column(nColumn);
	STGTYPE eType     = oColumn.StgType();
	bool    bNullable = oColumn.Nullable();
	CValue  oSum(null);

	ASSERT( (eType == MDST_INT) || (eType == MDST_INT64) || (eType == MDST_DOUBLE) );

	// Initialise the result.
	if      (eType == MDST_INT)		oSum = CValue(0);
	else if (eType == MDST_INT64)	oSum = CValue(0LL);
	else if (eType == MDST_DOUBLE)	oSum = CValue(0.0);

	// Sum the rows.
	for (size_t i = 0; i < Count(); ++i)
	{
		const CField& oField = (*this)[i][nColumn];

		// Ignore null values.
		if ( (bNullable) && (oField == null) )
			continue;

		if      (eType == MDST_INT)		oSum.m_iValue += oField.GetInt();
		else if (eType == MDST_INT64)	oSum.m_i64Value += oField.GetInt64();
		else if (eType == MDST_DOUBLE)	oSum.m_dValue += oField.GetDouble();
	}

	return oSum;
}

CValue CResultSet::Min(size_t nColumn) const
{
	const CColumn& oColumn = m_pTable->Column(nColumn);
	bool    bNullable = oColumn.Nullable();
	CValue  oSum(null);

	// For all rows.
	for (size_t i = 0; i < Count(); ++i)
	{
		const CField& oField = (*this)[i][nColumn];

		// Ignore null values.
		if ( (bNullable) && (oField == null) )
			continue;

		// Is first non-null value OR smaller?
		if ( (oSum.m_bNull) || (oField.Compare(oSum) < 0) )
			oSum = oField.ToValue();
	}

	return oSum;
}

CValue CResultSet::Max(size_t nColumn) const
{
	const CColumn& oColumn = m_pTable->Column(nColumn);
	bool    bNullable = oColumn.Nullable();
	CValue  oSum(null);

	// For all rows.
	for (size_t i = 0; i < Count(); ++i)
	{
		const CField& oField = (*this)[i][nColumn];

		// Ignore null values.
		if ( (bNullable) && (oField == null) )
			continue;

		// Is first non-null value OR smaller?
		if ( (oSum.m_bNull) || (oField.Compare(oSum) > 0) )
			oSum = oField.ToValue();
	}

	return oSum;
}

/******************************************************************************
** Method:		Distinct()
**
** Description:	Finds the distinct set of values for a column.
**
** Parameters:	nColumn		The index of the column to search.
**
** Returns:		The set of values.
**
*******************************************************************************
*/

CValueSet CResultSet::Distinct(size_t nColumn) const
{
	CValueSet oSet;

	// Result set not empty?
	if (Count() > 0)
	{
		CResultSet    oRS(*this);
		const CField* pCurrValue = nullptr;

		// Sort a copy of the result set.
		oRS.OrderBy(nColumn, CSortColumns::ASC);

		// For all rows.
		for (size_t i = 0; i < oRS.Count(); ++i)
		{
			CRow& oRow = oRS[i];

			// First value OR row value differs?
			if ( (pCurrValue == nullptr) || (oRow[nColumn] != *pCurrValue) )
			{
				oSet.Add(oRow[nColumn].ToValue());
				pCurrValue = &oRow[nColumn];
			}
		}
	}

	return oSet;
}

/******************************************************************************
** Method:		GroupBy()
**
** Description:	Groups the rows by the given column.
**
** Parameters:	nColumn		The index of the column to group on.
**
** Returns:		The set of grouped rows.
**
*******************************************************************************
*/

CGroupSet CResultSet::GroupBy(size_t nColumn) const
{
	CGroupSet oGS;

	// Simple set?
	if (Count() < 2)
	{
		if (Count() == 1)
			oGS.Add(*this);

		return oGS;
	}

	CResultSet oRS(*this);

	// Sort by the column.
	oRS.OrderBy(nColumn, CSortColumns::ASC);

	// Create a set for the group.
	CResultSet oGroup(*m_pTable);

	// Start the first group.
	oGroup.Add(oRS[0]);

	// For all subsequent rows
	for (size_t i = 1; i < oRS.Count(); ++i)
	{
		// Start of new group?
		if (oRS[i][nColumn] != oGroup[0][nColumn])
		{
			// Add group to set and clear.
			oGS.Add(oGroup);
			oGroup.Truncate();
		}

		oGroup.Add(oRS[i]);
	}

	// Add the final group to the set.
	oGS.Add(oGroup);

	return oGS;
}

/******************************************************************************
** Method:		Select()
**
** Description:	Runs a generic SELECT query on the set.
**
** Parameters:	oWhere	The where clause.
**
** Returns:		The result set.
**
*******************************************************************************
*/

CResultSet CResultSet::Select(const CWhere& oQuery) const
{
	CResultSet oRS(*m_pTable);

	// For all rows, apply the clause,
	for (size_t i = 0; i < Count(); ++i)
	{
		CRow& oRow = Row(i);

		if (oQuery.Matches(oRow))
			oRS.Add(oRow);
	}

	return oRS;
}

/******************************************************************************
** Method:		Exists()
**
** Description:	Queries if at least one row matches the WHERE clause.
**
** Parameters:	oWhere	The where clause.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CResultSet::Exists(const CWhere& oQuery) const
{
	// For all rows, apply the clause,
	for (size_t i = 0; i < Count(); ++i)
	{
		CRow& oRow = Row(i);

		if (oQuery.Matches(oRow))
			return true;
	}

	return false;
}

/******************************************************************************
** Method:		Dump()
**
** Description:	Dump the contents of the result set to the stream as text.
**
** Parameters:	rStream		The stream to dump into.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CResultSet::Dump(WCL::IOutputStream& rStream) const
{
	ASSERT(m_pTable != nullptr);

	std::vector<size_t>	aiColWidths;
	CString			strColList;
	size_t			nRowWidth = 0;

	// Get the column widths and name list.
	for (size_t i = 0; i < m_pTable->ColumnCount(); ++i)
	{
		const CColumn& oColumn = m_pTable->Column(i);

		// Get the column value width and name.
		size_t  nWidth   = oColumn.DisplayWidth(true);
		CString strName  = oColumn.Name();
		size_t  nNameLen = strName.Length();

		// Truncate long fields to 20 chars.
		if (nWidth > 20)
			nWidth = 20;

		// Name longer than value?
		if (nNameLen > nWidth)
		{
			// Truncate name.
			strName = strName.Left(nWidth);
		}
		// Value longer than name?
		else if (nWidth > nNameLen)
		{
			size_t nPadChars = (nWidth - nNameLen);
			size_t nBytes    = Core::numBytes<tchar>(nPadChars+1);

			// Create padding with spaces.
			tchar* pszPad = static_cast<tchar*>(_alloca(nBytes));
			std::fill(pszPad, pszPad+nPadChars, TXT(' '));
			pszPad[nPadChars] = TXT('\0');

			// Pad out name.
			strName += pszPad;
		}

		// Write column name and separator.
		rStream.Write(strName, nWidth);
		rStream.Write(" ", 1);

		// Track widths.
		aiColWidths.push_back(nWidth);
		nRowWidth += ++nWidth;
	}

	// Write EOL.
	rStream.Write("\r\n", 2);

	// Create the heading underline.
	tchar* psUnderline = static_cast<tchar*>(_alloca(Core::numBytes<tchar>(nRowWidth)));
	std::fill(psUnderline, psUnderline+nRowWidth, TXT('='));

	for (size_t j = 0, pos = 0; j < aiColWidths.size(); ++j)
	{
		pos += aiColWidths[j];
		psUnderline[pos++] = TXT(' ');
	}

	// Write heading underline.
	rStream.Write(psUnderline, nRowWidth);
	rStream.Write("\r\n", 2);

	// Reuse underline for padding.
	tchar* psPad = psUnderline;
	std::fill(psPad, psPad+nRowWidth, TXT(' '));

	// For all rows.
	for (size_t r = 0; r < m_pTable->RowCount(); ++r)
	{
		CRow& oRow = Row(r);

		// For all columns in the row.
		for (size_t c = 0; c < m_pTable->ColumnCount(); c++)
		{
			CString strValue  = oRow[c].DbgFormat();
			size_t  nValueLen = strValue.Length();
			size_t  nColWidth = aiColWidths[c];

			// Value longer than field?
			if (nValueLen > nColWidth)
			{
				// Write truncated value.
				rStream.Write(strValue, nColWidth);
			}
			else if (nColWidth > nValueLen)
			{
				// Write value and padding.
				rStream.Write(strValue, nValueLen);
				rStream.Write(psPad, (nColWidth-nValueLen));
			}
			else // nColWidth == nValueLen
			{
				// Write value and padding.
				rStream.Write(strValue, nValueLen);
			}

			rStream.Write(" ", 1);
		}

		// Write EOL.
		rStream.Write("\r\n", 2);
	}

	// Write EOL.
	rStream.Write("\r\n", 2);
}
