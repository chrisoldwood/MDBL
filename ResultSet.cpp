/******************************************************************************
**
** MODULE:		RESULTSET.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CResultSet class definition.
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

CResultSet::CResultSet()
	: m_pTable(NULL)
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
	if (pRow != NULL)
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
	: m_pTable(oResultSet.m_pTable)
{
	Reserve(oResultSet.Count());

	for (int i = 0; i < oResultSet.Count(); i++)
		Add(oResultSet[i]);
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
	Reserve(oRowSet.Count());

	for (int i = 0; i < oRowSet.Count(); i++)
		Add(oRowSet[i]);
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
** Method:		OrderBy()
**
** Description:	Sort the result by the column specified.
**				NB: Currently uses a bubble sort.
**
** Parameters:	oColumns	The columns and orders to sort by.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CResultSet::OrderBy(const CSortColumns& oColumns)
{
	int nRows = Count();
	int nExgs = 1;

	// For all rows OR until no exchanges were done.
	for (int i = 0; (i < nRows) && (nExgs > 0); i++)
	{
		nExgs = 0;

		// For all row comparisions.
		for (int j = 1; j < nRows; j++)
		{
			// Compare the next two rows.
			CRow* pRow1 = (CRow*) CPtrArray::Item(j-1);
			CRow* pRow2 = (CRow*) CPtrArray::Item(j);
			bool  bSwap = false;

			// Compare all columns.
			for (int k = 0; k < oColumns.Count(); k++)
			{
				// Get the column and direction.
				int nColumn = oColumns.Column(k);
				int nDir    = oColumns.Direction(k);

				int nCmp = pRow1->Field(nColumn).Compare(pRow2->Field(nColumn));

				// Wrong order?
				if ( ((nCmp > 0) && (nDir == CSortColumns::ASC))
				  || ((nCmp < 0) && (nDir == CSortColumns::DESC)) )
				{
					bSwap = true;
					break;
				}

				// Not equal?
				if (nCmp != 0)
					break;
			}

			// Swap required?
			if (bSwap)
			{
				CPtrArray::Set(j-1, pRow2);
				CPtrArray::Set(j,   pRow1);

				nExgs++;
			}
		}
	}
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

void CResultSet::Dump(CStream& rStream) const
{
	ASSERT(m_pTable != NULL);

	CIntArray	aiColWidths;
	CString		strColList;
	int			nRowWidth = 0;

	// Get the column widths and name list.
	for (int i = 0; i < m_pTable->ColumnCount(); i++)
	{
		const CColumn& oColumn = m_pTable->Column(i);

		// Get the column value width and name.
		int     nWidth   = oColumn.DisplayWidth();
		CString strName  = oColumn.Name();
		int     nNameLen = strName.Length();

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
			int nPadChars = (nWidth - nNameLen);

			// Create padding with spaces.
			char* pszPad = (char*) _alloca(nPadChars+1);
			memset(pszPad, ' ', nPadChars);
			pszPad[nPadChars] = '\0';

			// Pad out name.
			strName += pszPad;
		}

		// Write column name and separator.
		rStream.Write(strName, nWidth);
		rStream.Write(" ", 1);

		// Track widths.
		aiColWidths.Add(nWidth);
		nRowWidth += ++nWidth;
	}

	// Write EOL.
	rStream.Write("\r\n", 2);

	// Create the heading underline.
	char* psUnderline = (char*) _alloca(nRowWidth);
	memset(psUnderline, '=', nRowWidth);

	for (int j = 0, pos = 0; j < aiColWidths.Size(); j++)
	{
		pos += aiColWidths[j];
		psUnderline[pos++] = ' ';
	}

	// Write heading underline.
	rStream.Write(psUnderline, nRowWidth);
	rStream.Write("\r\n", 2);

	// Reuse underline for padding.
	char* psPad = psUnderline;
	memset(psPad, ' ', nRowWidth);

	// For all rows.
	for (int r = 0; r < m_pTable->RowCount(); r++)
	{
		CRow& oRow = Row(r);

		// For all columns in the row.
		for (int c = 0; c < m_pTable->ColumnCount(); c++)
		{
			CString strValue  = oRow[c].DbgFormat();
			int     nValueLen = strValue.Length();
			int     nColWidth = aiColWidths[c];

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
