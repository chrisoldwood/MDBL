/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TABLE.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CTable class definition.
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

CTable::CTable(CMDB& oDB, const char* pszName, bool bTemp)
	: m_oDB(oDB)
	, m_strName(pszName)
	, m_bTemp(bTemp)
	, m_bInserted(false)
	, m_bDeleted(false)
	, m_nIdentCol(-1)
	, m_nIdentVal(0)
{
	ASSERT(pszName != NULL);
	ASSERT(oDB.FindTable(pszName) == -1);
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

CTable::~CTable()
{
}

/******************************************************************************
** Method:		AddColumn()
**
** Description:	Appends a data column to the table.
**
** Parameters:	pszName		The name.
**				eType		The data type.
**				nLength		The length, if applicable.
**				nFlags		Additional flags.
**
** Returns:		The index of the new column.
**
*******************************************************************************
*/

int CTable::AddColumn(const char* pszName, COLTYPE eType, int nLength, int nFlags)
{
	ASSERT(m_vRows.Size() == 0);
	ASSERT(FindColumn(pszName) == -1);
	ASSERT(!((eType == MDCT_IDENTITY) && (m_nIdentCol != -1)));

	CColumn* pColumn = NULL;

	switch (eType)
	{
		case MDCT_INT:
			pColumn = new CColumn(*this, pszName, MDCT_INT,       0,       sizeof(int),        nFlags);
			break;

		case MDCT_DOUBLE:	
			pColumn = new CColumn(*this, pszName, MDCT_DOUBLE,    0,       sizeof(double),     nFlags);
			break;

		case MDCT_CHAR:		
			pColumn = new CColumn(*this, pszName, MDCT_CHAR,      1,       sizeof(char),       nFlags);
			break;

		case MDCT_FXDSTR:	
			pColumn = new CColumn(*this, pszName, MDCT_FXDSTR,    nLength, nLength+1,          nFlags);
			break;

//		case MDCT_VARSTR:	
//			pColumn = new CColumn(*this, pszName, MDCT_FXDSTR,    nLength, sizeof(char*),      nFlags);
//			break;

		case MDCT_BOOL:		
			pColumn = new CColumn(*this, pszName, MDCT_BOOL,      0,       sizeof(bool),       nFlags);
			break;

		case MDCT_IDENTITY:	
			pColumn = new CColumn(*this, pszName, MDCT_IDENTITY,  0,       sizeof(int),        CColumn::IDENTITY);
			break;

		case MDCT_DATETIME:
			pColumn = new CColumn(*this, pszName, MDCT_DATETIME,  0,       sizeof(time_t),     nFlags);
			break;

		case MDCT_TIMESTAMP:
			pColumn = new CColumn(*this, pszName, MDCT_TIMESTAMP, 0,       sizeof(CTimeStamp), nFlags);
			break;

		default:
			ASSERT(false);
			break;

	}

	ASSERT(pColumn != NULL);

	// Add to the table.
	int i = m_vColumns.Add(*pColumn);

	// Is identity column?
	if (pColumn->ColType() == MDCT_IDENTITY)
	{
		m_nIdentCol = i;

		// Column is indexed by default.
		pColumn->Index(new CIntIndex(*this, i, true));
	}

	return i;
}

/******************************************************************************
** Method:		AddColumn()
**
** Description:	Appends a foreign key column to the table.
**
** Parameters:	pszName		The name.
**				nFlags		The flags.
**				oTable		The foreign table.
**				nColumn		The foreign tables' colummn.
**
** Returns:		The index of the new column.
**
*******************************************************************************
*/

int CTable::AddColumn(const char* pszName, int nFlags, CTable& oTable, int nColumn)
{
	ASSERT(m_vRows.Size() == 0);
	ASSERT(FindColumn(pszName) == -1);
	ASSERT(oTable.m_vColumns[nColumn].Unique());
	ASSERT(oTable.m_vColumns[nColumn].Index() != NULL);

	// Create the column based on the foreign columns details.
	CColumn* pColumn = new CColumn(*this, pszName, nFlags, oTable, nColumn, oTable.m_vColumns[nColumn]);

	// Add to the table.
	int i = m_vColumns.Add(*pColumn);

	return i;
}

/******************************************************************************
** Method:		DropColumn()
**
** Description:	Drop the specified column.
**
** Parameters:	nColumn		The colummn to drop.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::DropColumn(int nColumn)
{
	ASSERT(m_vRows.Size() == 0);

	// Drop the column.
	m_vColumns.Delete(nColumn);
}

/******************************************************************************
** Method:		AddColumn()
**
** Description:	Appends a foreign key column to the table.
**
** Parameters:	pszName		The name.
**				oTable		The foreign table.
**				nColumn		The foreign tables' colummn.
**
** Returns:		The index of the new column.
**
*******************************************************************************
*/

void CTable::DropAllColumns()
{
	ASSERT(m_vRows.Size() == 0);

	// Drop the columns.
	m_vColumns.DeleteAll();
}

/******************************************************************************
** Method:		AddIndex()
**
** Description:	Adds an index for a column.
**
** Parameters:	nColumn			The column to index.
**				eType			The type of index to use.
**				nApproxRows		The expected number of rows.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::AddIndex(int nColumn, CIndex::Type eIndexType, int nApproxRows)
{
	ASSERT(m_vRows.Size() == 0);
	ASSERT(m_vColumns[nColumn].Index() == NULL);

	CIndex* pIndex = NULL;

	// Get column details.
	const CColumn& oColumn = m_vColumns[nColumn];

	COLTYPE eColType = oColumn.ColType();
	bool    bUnique  = oColumn.Unique();

	switch (eColType)
	{
		case MDCT_INT:		pIndex = new CIntIndex(*this, nColumn, bUnique);	break;
		case MDCT_IDENTITY:	pIndex = new CIntIndex(*this, nColumn, true);		break;
		default:			ASSERT(false);										break;
	}

	ASSERT(pIndex != NULL);

	m_vColumns[nColumn].Index(pIndex);
}

/******************************************************************************
** Method:		DropIndex()
**
** Description:	Drops an index from a column.
**
** Parameters:	nColumn		The index to drop.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::DropIndex(int nColumn)
{
	ASSERT(m_vColumns[nColumn].ColType() != MDCT_IDENTITY);

	m_vColumns[nColumn].Index(NULL);
}

/******************************************************************************
** Method:		CreateRow()
**
** Description:	Allocates a new row.
**
** Parameters:	None.
**
** Returns:		The allocated row.
**
*******************************************************************************
*/

CRow& CTable::CreateRow()
{
	return *(new CRow(*this));
}

/******************************************************************************
** Method:		InsertRow()
**
** Description:	Inserts a row into the table. If the table contains an identity
**				column, its value is set.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

int CTable::InsertRow(CRow& oRow)
{
	ASSERT(&oRow.Table()   == this);
	ASSERT(oRow.Inserted() == false);

	// Call "trigger".
	OnBeforeInsert(oRow);

	// Set the identity value, if one.
	if (m_nIdentCol != -1)
		oRow[m_nIdentCol] = ++m_nIdentVal;

#ifdef _DEBUG
	// Check null flags.
	for (int n = 0; n < m_vColumns.Size(); n++)
	{
		bool bCanBeNull = m_vColumns[n].Nullable();
		bool bIsNull    = (oRow[n] == null);

		ASSERT(!(!bCanBeNull && bIsNull));
	}
#endif //_DEBUG

	// Update any indexes.
	for (int i=0; i < m_vColumns.Size(); i++)
	{
		CIndex* pIndex = m_vColumns[i].Index();

		if (pIndex != NULL)
			pIndex->AddRow(oRow);
	}

	// Mark as inserted.
	oRow.Status(CRow::INSERTED);
	m_bInserted = true;

	// Append it.
	int nRow = m_vRows.Add(oRow);

	// Call "trigger".
	OnAfterInsert(oRow);

	return nRow;
}

/******************************************************************************
** Method:		DeleteRow()
**
** Description:	Deletes a row using the number of the row.
**
** Parameters:	nRow	The row number.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::DeleteRow(int nRow)
{
	CRow& oRow = m_vRows[nRow];

	// Call "trigger".
	OnBeforeDelete(oRow);

	// Update any indexes.
	for (int i=0; i < m_vColumns.Size(); i++)
	{
		CIndex* pIndex = m_vColumns[i].Index();

		if (pIndex != NULL)
			pIndex->RemoveRow(oRow);
	}

	// Remove it.
	m_vRows.Remove(nRow);
	m_bDeleted = true;

	// Call "trigger".
	OnAfterDelete(oRow);

	// Free resources.
	delete &oRow;
}

/******************************************************************************
** Method:		DeleteRow()
**
** Description:	Deletes a row using the row iteslf.
**
** Parameters:	oRow	The row to delete.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::DeleteRow(CRow& oRow)
{
	for (int i=0; i < m_vRows.Size(); i++)
	{
		if (&m_vRows[i] == &oRow)
		{
			DeleteRow(i);
			break;
		}
	}
}

/******************************************************************************
** Method:		Truncate()
**
** Description:	Deletes all rows in the table.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::Truncate()
{
	// Anything to truncate?
	if (m_vRows.Size() > 0)
	{
		// Update any indexes.
		for (int i=0; i < m_vColumns.Size(); i++)
		{
			CIndex* pIndex = m_vColumns[i].Index();

			if (pIndex != NULL)
				pIndex->Truncate();
		}

		// Remove all.
		m_vRows.DeleteAll();
		m_bDeleted = true;
	}
}

/******************************************************************************
** Method:		SelectRow()
**
** Description:	Selects the first row from the table where the column matches
**				the given value. This method expects to use an index.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CRow* CTable::SelectRow(int nColumn, const CValue& oValue) const
{
	ASSERT(oValue.m_bNull == false);
	ASSERT(m_vColumns[nColumn].Unique());
	ASSERT(m_vColumns[nColumn].Index() != NULL);

	// Use index, to find it.
	return m_vColumns[nColumn].Index()->FindRow(oValue);
}

/******************************************************************************
** Method:		Select()
**
** Description:	Runs a generic SELECT query on the table.
**
** Parameters:	oWhere	The where clause.
**
** Returns:		The result set.
**
*******************************************************************************
*/

CResultSet CTable::Select(const CWhere& oWhere) const
{
	CResultSet oRS;

	// For all rows, apply the clause,
	for (int i = 0; i < m_vRows.Size(); i++)
	{
		CRow& oRow = m_vRows[i];

		if (oWhere.Matches(oRow))
			oRS.Add(oRow);
	}

	return oRS;
}

/******************************************************************************
** Method:		Modified()
**
** Description:	Checks if there have been any insertions or deletions and then
**				if any rows have been updated.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTable::Modified() const
{
	// Ignore if temporary table.
	if (m_bTemp)
		return false;

	// Any insertions or deletions?
	if (m_bInserted || m_bDeleted)
		return true;

	// Check all rows.
	return m_vRows.Modified();
}

/******************************************************************************
** Methods:		operator <<()
**				operator >>()
**
** Description:	Operators to read/write the data from/to a stream.
**
** Parameters:	rStream		The stream.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::operator <<(CStream& rStream)
{
	m_vRows.DeleteAll();

	// Is a temporary table?
	if (m_bTemp)
	{
		// Reset modified flags.
		m_bInserted = false;
		m_bDeleted  = false;

		return;
	}

	int32 nRows;

	// Read the row count.
	rStream >> nRows;

	// Read the actual rows.
	for (int i = 0; i < nRows; i++)
	{
		CRow& oRow = CTable::CreateRow();

		oRow << rStream;

#ifdef _DEBUG
		// Check null flags and foreign keys.
		for (int k = 0; k < m_vColumns.Size(); k++)
		{
			bool bCanBeNull = m_vColumns[k].Nullable();
			bool bIsNull    = (oRow[k] == null);

			ASSERT(!(!bCanBeNull && bIsNull));

			CTable* pFKTable  = m_vColumns[k].FKTable();
			int     nFKColumn = m_vColumns[k].FKColumn();

			if (pFKTable == NULL)
				continue;

			ASSERT(pFKTable->SelectRow(nFKColumn, oRow[k]) != NULL);
		}
#endif //_DEBUG

		m_vRows.Add(oRow);

		// Update any indexes.
		for (int n = 0; n < m_vColumns.Size(); n++)
		{
			CIndex* pIndex = m_vColumns[n].Index();

			if (pIndex != NULL)
				pIndex->AddRow(oRow);
		}
	}

	// Read the identity value.
	rStream >> m_nIdentVal;

	// Reset modified flags.
	m_bInserted = false;
	m_bDeleted  = false;
}

void CTable::operator >>(CStream& rStream)
{
	// Is a temporary table?
	if (m_bTemp)
	{
		// Reset modified flags.
		m_bInserted = false;
		m_bDeleted  = false;

		return;
	}

	int32 nRows = m_vRows.Size();

	// Write the row count.
	rStream << nRows;

	// Write the actual rows.
	for (int i = 0; i < nRows; i++)
		m_vRows[i] >> rStream;

	// Write the identity value.
	rStream << m_nIdentVal;

	// Reset modified flags.
	m_bInserted = false;
	m_bDeleted  = false;
}

/******************************************************************************
** Methods:		operator <<()
**				operator >>()
**
** Description:	Operators to read/write the data from/to a Database.
**
** Parameters:	rSource		The data source.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::operator <<(CSQLSource& rSource)
{
	ASSERT(rSource.IsOpen());

	CString strBaseQuery = "SELECT %s FROM %s";
	CString strColumns;
	CString strQuery;

	// Get column list.
	for (int i = 0; i < m_vColumns.Size(); i++)
	{
		if (i > 0) strColumns += ',';
		
		strColumns += m_vColumns[i].Name();
	}

	strQuery.Format(strBaseQuery, strColumns, Name());

	CSQLCursor*	pCursor = NULL;

	try
	{
		pCursor = rSource.ExecQuery(strQuery);

		// Set the output column types.
		for (int i = 0; i < m_vColumns.Size(); i++)
		{
			CColumn&   oTabColumn = m_vColumns[i];
			SQLColumn& oSQLColumn = pCursor->Column(i);

			ASSERT(oTabColumn.Name()     == oSQLColumn.m_strName);
			ASSERT(oTabColumn.Nullable() == (oSQLColumn.m_nFlags & CColumn::NULLABLE));
			ASSERT(!((oTabColumn.ColType() == MDCT_FXDSTR) && (oTabColumn.Length() < oSQLColumn.m_nSize)));

			oSQLColumn.m_eMDBColType = oTabColumn.ColType();
			oSQLColumn.m_nSize       = oTabColumn.Length();
		}

		// For all rows.
		while (pCursor->Fetch())
		{
			// Allocate the row.
			CRow& oRow = CreateRow();

			// Copy the data.
			pCursor->SetRow(oRow);

			// Append to table.
			InsertRow(oRow);
			oRow.Status(CRow::ORIGINAL);
		}

		// Cleanup.
		delete pCursor;
	}
	catch(CSQLException&)
	{
		delete pCursor;
		throw;
	}
}

void CTable::operator >>(CSQLSource& rSource)
{
}

/******************************************************************************
** Methods:		OnBefore*()
**				OnAfter*()
**
** Description:	Template methods called before and after a row operation.
**
** Parameters:	oRow	The row being affected.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::OnBeforeInsert(CRow& oRow)
{
}

void CTable::OnAfterInsert(CRow& oRow)
{
}

void CTable::OnBeforeDelete(CRow& oRow)
{
}

void CTable::OnAfterDelete(CRow& oRow)
{
}
