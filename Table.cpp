/******************************************************************************
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

CTable::CTable(CMDB& oDB, const char* pszName, int nFlags)
	: m_oDB(oDB)
	, m_strName(pszName)
	, m_nFlags(nFlags)
	, m_nInsertions(0)
	, m_nUpdates(0)
	, m_nDeletions(0)
	, m_nIdentCol(-1)
	, m_nIdentVal(0)
	, m_pNullRow(NULL)
{
	ASSERT(pszName != NULL);
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
	delete m_pNullRow;
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
	ASSERT(m_vRows.Count() == 0);
	ASSERT(FindColumn(pszName) == -1);
	ASSERT(!((eType == MDCT_IDENTITY) && (m_nIdentCol != -1)));

	// Apply table settings to all columns.
	if (ReadOnly())		nFlags |= CColumn::READ_ONLY;
	if (Transient())	nFlags |= CColumn::TRANSIENT;

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

		case MDCT_VARSTR:
			pColumn = new CColumn(*this, pszName, MDCT_VARSTR,    nLength, 0,                  nFlags);
			break;

		case MDCT_BOOL:
			pColumn = new CColumn(*this, pszName, MDCT_BOOL,      0,       sizeof(bool),       nFlags);
			break;

		case MDCT_IDENTITY:
			pColumn = new CColumn(*this, pszName, MDCT_IDENTITY,  0,       sizeof(int),        CColumn::IDENTITY);
			break;

		case MDCT_DATETIME:
			pColumn = new CColumn(*this, pszName, MDCT_DATETIME,  0,       sizeof(time_t),     nFlags);
			break;

		case MDCT_DATE:
			pColumn = new CColumn(*this, pszName, MDCT_DATE,      0,       sizeof(time_t),     nFlags);
			break;

		case MDCT_TIME:
			pColumn = new CColumn(*this, pszName, MDCT_TIME,      0,       sizeof(time_t),     nFlags);
			break;

		case MDCT_TIMESTAMP:
			pColumn = new CColumn(*this, pszName, MDCT_TIMESTAMP, 0,       sizeof(CTimeStamp), nFlags);
			break;

		case MDCT_VOIDPTR:
			pColumn = new CColumn(*this, pszName, MDCT_VOIDPTR,   0,       0,                  nFlags);
			break;

		case MDCT_ROWPTR:
			pColumn = new CColumn(*this, pszName, MDCT_ROWPTR,    0,       0,                  nFlags);
			break;

		case MDCT_ROWSETPTR:
			pColumn = new CColumn(*this, pszName, MDCT_ROWSETPTR, 0,       0,                  nFlags);
			break;

		default:
			ASSERT_FALSE();
			break;

	}

	ASSERT(pColumn != NULL);

	// Add to the table.
	int i = m_vColumns.Add(*pColumn);

	// Is identity column?
	if (pColumn->ColType() == MDCT_IDENTITY)
		m_nIdentCol = i;

	// If unique add index.
	if (nFlags & CColumn::UNIQUE)
		AddIndex(i);

	return i;
}

/******************************************************************************
** Method:		AddColumn()
**
** Description:	Appends a foreign key column to the table.
**
** Parameters:	pszName		The name.
**				oTable		The foreign table.
**				nColumn		The foreign tables' colummn.
**				nFlags		The flags.
**
** Returns:		The index of the new column.
**
*******************************************************************************
*/

int CTable::AddColumn(const char* pszName, CTable& oTable, int nColumn, int nFlags)
{
	ASSERT(m_vRows.Count() == 0);
	ASSERT(FindColumn(pszName) == -1);
	ASSERT(oTable.m_vColumns[nColumn].Unique());
	ASSERT(oTable.m_vColumns[nColumn].Index() != NULL);
	ASSERT(nFlags & CColumn::FOREIGN_KEY);

	// Create the column based on the foreign columns details.
	CColumn* pColumn = new CColumn(*this, pszName, oTable, nColumn, oTable.m_vColumns[nColumn], nFlags);

	// Add to the table.
	int i = m_vColumns.Add(*pColumn);

	// If unique add index.
	if (nFlags & CColumn::UNIQUE)
		AddIndex(i);

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
	ASSERT(m_vRows.Count() == 0);

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
	ASSERT(m_vRows.Count() == 0);

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

void CTable::AddIndex(int nColumn)
{
	ASSERT(m_vRows.Count() == 0);
	ASSERT(m_vColumns[nColumn].Index() == NULL);

	CIndex* pIndex = NULL;

	// Get column details.
	const CColumn& oColumn = m_vColumns[nColumn];

	COLTYPE eColType = oColumn.ColType();
	bool    bUnique  = oColumn.Unique();

	switch (eColType)
	{
		case MDCT_INT:	
			if (bUnique)
				pIndex = new CIntMapIndex(*this, nColumn);
			break;

		case MDCT_FXDSTR:
			if (bUnique)
				pIndex = new CStrMapIndex(*this, nColumn);
			break;

		case MDCT_VARSTR:
			if (bUnique)
				pIndex = new CStrMapIndex(*this, nColumn);
			break;

		case MDCT_IDENTITY:
			ASSERT(bUnique);
			pIndex = new CIntMapIndex(*this, nColumn);
			break;

		default:
			ASSERT_FALSE();
			break;
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
** Parameters:	oRow	The row to insert.
**				bNew	Is a new row or being serialized in?
**
** Returns:		Nothing.
**
*******************************************************************************
*/

int CTable::InsertRow(CRow& oRow, bool bNew)
{
	ASSERT(&oRow.Table()   == this);
	ASSERT(oRow.InTable() == false);

	// Call "trigger".
	OnBeforeInsert(oRow);

	// Set the identity value, if one.
	if (m_nIdentCol != -1)
		oRow[m_nIdentCol] = ++m_nIdentVal;

#ifdef _DEBUG
	// Check row nulls and fkeys.
	CheckRow(oRow, false);

	// Check index sizes.
	CheckIndexes();
#endif //_DEBUG

	// Update any indexes.
	for (int i=0; i < m_vColumns.Count(); i++)
	{
		CIndex* pIndex = m_vColumns[i].Index();

		if (pIndex != NULL)
			pIndex->AddRow(oRow);
	}

	// New row?
	if (bNew)
	{
		oRow.MarkInserted();
		m_nInsertions++;
	}
	// Serialised row.
	else
	{
		oRow.MarkOriginal();
	}

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
	for (int i=0; i < m_vColumns.Count(); i++)
	{
		CIndex* pIndex = m_vColumns[i].Index();

		if (pIndex != NULL)
			pIndex->RemoveRow(oRow);
	}

	// Remove it.
	m_vRows.Remove(nRow);
	m_nDeletions++;

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
	for (int i=0; i < m_vRows.Count(); i++)
	{
		if (&m_vRows[i] == &oRow)
		{
			DeleteRow(i);
			break;
		}
	}
}

/******************************************************************************
** Method:		DeleteRows()
**
** Description:	Deletes the rows in the result set from the table.
**
** Parameters:	oRS		The set of rows to delete.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::DeleteRows(const CResultSet& oRS)
{
	for (int i =0; i < oRS.Count(); i++)
		DeleteRow(oRS[i]);
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
	if (m_vRows.Count() > 0)
	{
		// Remove all.
		m_vRows.DeleteAll();
		TruncateIndexes();
	}
}

/******************************************************************************
** Method:		CopyTable()
**
** Description:	Copies all the rows in the source table.
**
** Parameters:	oTable	The source table.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::CopyTable(const CTable& oTable)
{
	ASSERT(ColumnCount() == oTable.ColumnCount());

	// For all rows...
	for (int r = 0; r < oTable.RowCount(); ++r)
	{
		CRow& oSrcRow = oTable[r];
		CRow& oDstRow = CreateRow();

		// For all columns...
		for (int c = 0; c < oTable.ColumnCount(); ++c)
			oDstRow[c] = oSrcRow[c];

		InsertRow(oDstRow);
	}
}

/******************************************************************************
** Method:		NullRow()
**
** Description:	Returns the NULL row for outer joins. This is a special row
**				where all fields are NULL.
**
** Parameters:	None.
**
** Returns:		The NULL row.
**
*******************************************************************************
*/

CRow& CTable::NullRow()
{
	// Create NUll row if not already.
	if (m_pNullRow == NULL)
		m_pNullRow = new CRow(*this, true);

	return *m_pNullRow;
}

/******************************************************************************
** Method:		TruncateIndexes()
**
** Description:	Deletes all rows in all indexes.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::TruncateIndexes()
{
	// For all indexes.
	for (int i=0; i < m_vColumns.Count(); i++)
	{
		CIndex* pIndex = m_vColumns[i].Index();

		if (pIndex != NULL)
			pIndex->Truncate();
	}
}

/******************************************************************************
** Method:		SelectAll()
**
** Description:	Selects all rows from the table.
**
** Parameters:	None.
**
** Returns:		The result set.
**
*******************************************************************************
*/

CResultSet CTable::SelectAll() const
{
	return CResultSet(*this, m_vRows);
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
	CUniqIndex* pIndex = static_cast<CUniqIndex*>(m_vColumns[nColumn].Index());

	return pIndex->FindRow(oValue);
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
	CResultSet oRS(*this);

	// For all rows, apply the clause,
	for (int i = 0; i < m_vRows.Count(); i++)
	{
		CRow& oRow = m_vRows[i];

		if (oWhere.Matches(oRow))
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

bool CTable::Exists(const CWhere& oWhere) const
{
	// For all rows, apply the clause,
	for (int i = 0; i < m_vRows.Count(); i++)
	{
		CRow& oRow = m_vRows[i];

		if (oWhere.Matches(oRow))
			return true;
	}

	return false;
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
	// Ignore if table transient or read-only.
	if (Transient() || ReadOnly())
		return false;

	// Any insertions/updates/deletions?
	if (m_nInsertions || m_nUpdates || m_nDeletions)
		return true;

	return false;
}

/******************************************************************************
** Method:		Modified()
**
** Description:	Sets or resets the table and row modified flags.
**
** Parameters:	bModified	Set or Reset flags?
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::Modified(bool bModified)
{
	if (bModified)
	{
		// Set modified flags.
		m_nInsertions++;
		m_nUpdates++;
		m_nDeletions++;
	}
	else
	{
		// Reset table and row flags.
		ResetRowFlags();
	}
}

/******************************************************************************
** Methods:		Read()
**				Write()
**
** Description:	Operators to read/write the data from/to a stream.
**
** Parameters:	rStream		The stream.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::Read(WCL::IInputStream& rStream)
{
	// Remove all existing rows.
	m_vRows.DeleteAll();
	TruncateIndexes();

	// Ignore if a temporary table.
	if (Transient())
		return;

	int32 nColumns;
	int32 nRows;

	// Verify the column count.
	rStream >> nColumns;

	ASSERT(m_vColumns.Count() == nColumns);

	// Read the row count.
	rStream >> nRows;

	// Prepare any indexes.
	for (int n = 0; n < m_vColumns.Count(); n++)
	{
		CIndex* pIndex = m_vColumns[n].Index();

		if (pIndex != NULL)
			pIndex->Capacity(nRows);
	}

	// Read the actual rows.
	for (int i = 0; i < nRows; i++)
	{
		CRow& oRow = CTable::CreateRow();

		oRow.Read(rStream);

#ifdef _DEBUG
		// Check row nulls and fkeys.
		CheckRow(oRow, false);
#endif //_DEBUG

		m_vRows.Add(oRow);

		// Update any indexes.
		for (int n = 0; n < m_vColumns.Count(); n++)
		{
			CIndex* pIndex = m_vColumns[n].Index();

			if (pIndex != NULL)
				pIndex->AddRow(oRow);
		}
	}

#ifdef _DEBUG
	// Check index sizes.
	CheckIndexes();
#endif //_DEBUG

	// Read the identity value.
	rStream.Read(&m_nIdentVal, sizeof(m_nIdentVal));

	// Reset modified flags.
	m_nInsertions = 0;
	m_nUpdates    = 0;
	m_nDeletions  = 0;
}

void CTable::Write(WCL::IOutputStream& rStream)
{
	// Ignore if a temporary table.
	if (Transient())
		return;

	int32 nColumns = m_vColumns.Count();

	// Write the column count.
	rStream << nColumns;

	int32 nRows = m_vRows.Count();

	// Write the row count.
	rStream << nRows;

	// Write the actual rows.
	for (int i = 0; i < nRows; i++)
		m_vRows[i].Write(rStream);

	// Write the identity value.
	rStream.Write(&m_nIdentVal, sizeof(m_nIdentVal));

	// Reset modified flags.
	m_nInsertions = 0;
	m_nUpdates    = 0;
	m_nDeletions  = 0;
}

/******************************************************************************
** Method:		SQLColumnList()
**
** Description:	Gets the comma separated column list.
**
** Parameters:	None.
**
** Returns:		The column list.
**
*******************************************************************************
*/

CString CTable::SQLColumnList() const
{
	CString strColumns;

	// Get column list.
	for (int i = 0; i < m_vColumns.Count(); i++)
	{
		// Ignore TRANSIENT columns.
		if (!m_vColumns[i].Transient())
		{
			if (!strColumns.Empty())
				strColumns += ',';

			strColumns += m_vColumns[i].Name();
		}
	}

	return strColumns;
}

/******************************************************************************
** Method:		SQLQuery()
**
** Description:	Gets the SQL query required to load the table,
**
** Parameters:	None.
**
** Returns:		The query.
**
*******************************************************************************
*/

CString CTable::SQLQuery() const
{
	ASSERT(Transient() == false);
	ASSERT(m_vColumns.Count() > 0);

	// Get the SQL table name, if different.
	CString strTable = (m_strSQLTable.Empty()) ? m_strName : m_strSQLTable;

	// Build basic SELECT query.
	CString strQuery = "SELECT " + SQLColumnList() + " FROM " + strTable;

	// Append WHERE, if set.
	if (m_strSQLWhere.Empty() == false)
		strQuery += " WHERE " + m_strSQLWhere;

	// Append GROUP BY, if set.
	if (m_strSQLGroup.Empty() == false)
		strQuery += " GROUP BY " + m_strSQLGroup;

	// Append ORDER BY, if set.
	if (m_strSQLOrder.Empty() == false)
		strQuery += " ORDER BY " + m_strSQLOrder;

	return strQuery;
}

/******************************************************************************
** Methods:		Read()
**				Write()
**
** Description:	Operators to read/write the data from/to a Database.
**
** Parameters:	rSource		The data source.
**				eRows		The type of rows to write.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::Read(CSQLSource& rSource)
{
	// Remove all existing rows.
	m_vRows.DeleteAll();
	TruncateIndexes();

	// Ignore if a temporary table.
	if (Transient())
		return;

	ASSERT(rSource.IsOpen());

	CSQLCursor*	pCursor = NULL;

	try
	{
		pCursor = rSource.ExecQuery(SQLQuery());

		// Set the output column types.
		for (int iTabCol = 0, iSQLCol = 0; iTabCol < m_vColumns.Count(); iTabCol++)
		{
			CColumn& oTabColumn = m_vColumns[iTabCol];

			// Ignore TRANSIENT columns.
			if (!oTabColumn.Transient())
			{
				SQLColumn& oSQLColumn = pCursor->Column(iSQLCol);

				ASSERT(oTabColumn.Name() == oSQLColumn.m_strName);
				ASSERT(!((oTabColumn.ColType() == MDCT_FXDSTR) && (oTabColumn.Length() < oSQLColumn.m_nSize)));

				oSQLColumn.m_nDstColumn  = iTabCol;
				oSQLColumn.m_eMDBColType = oTabColumn.ColType();
				oSQLColumn.m_nSize       = oTabColumn.Length();

				iSQLCol++;
			}
		}

		// For all rows.
		while (pCursor->Fetch())
		{
			// Allocate the row.
			CRow& oRow = CreateRow();

			// Copy the data.
			pCursor->GetRow(oRow);

			// Append to table.
			InsertRow(oRow, false);
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

void CTable::Write(CSQLSource& rSource, RowTypes eRows)
{
	// Ignore if a temporary table.
	if (Transient())
		return;

	ASSERT(m_vColumns.Count() > 0);
	ASSERT(rSource.IsOpen());

	// Write inserted rows AND there are some?
	if ( (eRows & INSERTED) && (m_nInsertions > 0) )
		WriteInsertions(rSource);

	// Write updated rows?
	if ( (eRows & UPDATED) && (m_nUpdates > 0) )
		WriteUpdates(rSource);

	// Write deleted rows?
	if ( (eRows & DELETED) && (m_nDeletions > 0) )
		WriteDeletions(rSource);
}

/******************************************************************************
** Method:		Write*()
**
** Description:	Internal methods to writes the row changes.
**
** Parameters:	rSource		The connection.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::WriteInsertions(CSQLSource& rSource)
{
	bool bAnyDirty = false;

	// Perform a quick check to see if
	// there is actually anything to write.
	for (int i = 0; (i < RowCount()) && (!bAnyDirty); i++)
	{
		CRow& oRow = m_vRows[i];

		if (oRow.Inserted() && !oRow.Deleted())
			bAnyDirty = true;
	}

	// Nothing to write?
	if (!bAnyDirty)
		return;

	CString strColumns;
	CString strParams;
	CString strQuery;
	int		nParams = 0;

	// Create column and parameter list.
	for (int i = 0; i < m_vColumns.Count(); i++)
	{
		// Ignore TRANSIENT columns.
		if (m_vColumns[i].Transient())
			continue;

		// Not first column?
		if (!strColumns.Empty())
		{
			strColumns += ", ";
			strParams  += ", ";
		}

		strColumns += m_vColumns[i].Name();
		strParams  += '?';

		nParams++;
	}

	// Create the full statement.
	strQuery.Format("INSERT INTO %s (%s) VALUES (%s)", Name(), strColumns, strParams);

	ASSERT(nParams > 0);

	CSQLParams* pParams = NULL;

	try
	{
		// Allocate the parameters object.
		pParams = rSource.CreateParams(strQuery, nParams);

		// Create parameter definitions.
		for (int iTabCol = 0, iSQLParam = 0; iTabCol < m_vColumns.Count(); iTabCol++)
		{
			const CColumn& oColumn = m_vColumns[iTabCol];

			// Ignore TRANSIENT columns.
			if (oColumn.Transient())
				continue;

			SQLParam& oParam = pParams->Param(iSQLParam);

			// Set the details.
			oParam.m_nSrcColumn  = iTabCol;
			oParam.m_eMDBColType = oColumn.ColType();
			oParam.m_nMDBColSize = oColumn.Length();

			iSQLParam++;
		}

		// For all rows.
		for (int i = 0; i < RowCount(); i++)
		{
			CRow& oRow = m_vRows[i];

			// Ignore if not inserted OR already deleted.
			if (!oRow.Inserted() || oRow.Deleted())
				continue;

			// Set the params and execute.
			pParams->SetRow(oRow);
			rSource.ExecStmt(strQuery, *pParams);
		}

		// Cleanup.
		delete pParams;
	}
	catch(CODBCException&)
	{
		// Cleanup and rethrow.
		delete pParams;

		throw;
	}
}

void CTable::WriteUpdates(CSQLSource& rSource)
{
	bool bAnyDirty = false;

	// Perform a quick check to see if
	// there is actually anything to write.
	for (int i = 0; (i < RowCount()) && (!bAnyDirty); i++)
	{
		CRow& oRow = m_vRows[i];

		if (oRow.Updated() && !(oRow.Inserted() || oRow.Deleted()) )
			bAnyDirty = true;
	}

	// Nothing to write?
	if (!bAnyDirty)
		return;

	// For all rows.
	for (int r = 0; r < RowCount(); r++)
	{
		CRow& oRow = m_vRows[r];

		// Ignore row if unchanged OR handled by insert OR delete.
		if (!oRow.Updated() || oRow.Inserted() || oRow.Deleted())
			continue;

		CString strModColumns;
		CString strPKColumns;
		CString strQuery;
		int		nParams = 0;

		// Create column and where clause list.
		for (int i = 0; i < m_vColumns.Count(); i++)
		{
			const CColumn& oColumn = m_vColumns[i];

			// Ignore TRANSIENT columns.
			if (oColumn.Transient())
				continue;

			// Value modified?
			if (oRow[i].Modified())
			{
				if (!strModColumns.Empty())
					strModColumns += ", ";

				strModColumns += m_vColumns[i].Name();
				strModColumns += " = ?";

				nParams++;
			}

			// Part of primary key?
			if (oColumn.PrimaryKey())
			{
				ASSERT(oRow[i].Modified() == false);

				if (!strPKColumns.Empty())
					strPKColumns += " AND ";

				strPKColumns += m_vColumns[i].Name();
				strPKColumns += " = ?";

				nParams++;
			}
		}

		// Create the full statement.
		strQuery.Format("UPDATE %s SET %s WHERE %s", Name(), strModColumns, strPKColumns);

		ASSERT(nParams > 0);

		CSQLParams* pParams = NULL;

		try
		{
			// Allocate the parameters object.
			pParams = rSource.CreateParams(strQuery, nParams);

			int iSQLParam = 0;

			// Create modified column parameter definitions.
			for (int iTabCol = 0; iTabCol < m_vColumns.Count(); iTabCol++)
			{
				const CColumn& oColumn = m_vColumns[iTabCol];

				// Ignore TRANSIENT columns.
				if (oColumn.Transient())
					continue;

				if (oRow[iTabCol].Modified())
				{
					SQLParam& oParam = pParams->Param(iSQLParam);

					// Set the details.
					oParam.m_nSrcColumn  = iTabCol;
					oParam.m_eMDBColType = oColumn.ColType();
					oParam.m_nMDBColSize = oColumn.Length();

					iSQLParam++;
				}
			}

			// Create primary key column parameter definitions.
			for (int iTabCol = 0; iTabCol < m_vColumns.Count(); iTabCol++)
			{
				const CColumn& oColumn = m_vColumns[iTabCol];

				// Ignore TRANSIENT columns.
				if (oColumn.Transient())
					continue;

				if (oColumn.PrimaryKey())
				{
					SQLParam& oParam = pParams->Param(iSQLParam);

					// Set the details.
					oParam.m_nSrcColumn  = iTabCol;
					oParam.m_eMDBColType = oColumn.ColType();
					oParam.m_nMDBColSize = oColumn.Length();

					iSQLParam++;
				}
			}

			// Set the params and execute.
			pParams->SetRow(oRow);
			rSource.ExecStmt(strQuery, *pParams);

			// Cleanup.
			delete pParams;
		}
		catch(CODBCException&)
		{
			// Cleanup and rethrow.
			delete pParams;

			throw;
		}
	}
}

void CTable::WriteDeletions(CSQLSource& /*rSource*/)
{
	ASSERT_FALSE();
}

/******************************************************************************
** Method:		ResetRowFlags()
**
** Description:	Resets the modified flag on all rows.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::ResetRowFlags()
{
	// Update all rows.
	for (int i = 0; i < RowCount(); i++)
		m_vRows[i].ResetStatus();

	// Update table counters.
	m_nInsertions = 0;
	m_nUpdates    = 0;
	m_nDeletions  = 0;
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

void CTable::OnBeforeInsert(CRow& /*oRow*/)
{
}

void CTable::OnAfterInsert(CRow& /*oRow*/)
{
}

void CTable::OnBeforeDelete(CRow& /*oRow*/)
{
}

void CTable::OnAfterDelete(CRow& /*oRow*/)
{
}

/******************************************************************************
** Method:		CheckIndexes()
**
** Description:	Checks the row of the indexes.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::CheckIndexes() const
{
#ifdef _DEBUG
	// For all indexes.
	for (int i=0; i < m_vColumns.Count(); i++)
	{
		CIndex* pIndex = m_vColumns[i].Index();

		if (pIndex == NULL)
			continue;

		ASSERT(pIndex->RowCount() == m_vRows.Count());
	}
#endif
}

// Unreferenced formal parameter.
#pragma warning ( disable : 4100 )

/******************************************************************************
** Method:		CheckRow()
**
** Description:	Checks the row is valid.
**
** Parameters:	oRow	The row to check.
**				bUpdate	Is an UPDATE or an INSERT?
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::CheckRow(CRow& oRow, bool bUpdate) const
{
#ifdef _DEBUG
	for (int k = 0; k < m_vColumns.Count(); k++)
	{
		bool bCanBeNull = m_vColumns[k].Nullable();
		bool bIsNull    = (oRow[k] == null);

		ASSERT(!(!bCanBeNull && bIsNull));

		CheckColumn(oRow, k, oRow[k], bUpdate);

		CTable* pFKTable  = m_vColumns[k].FKTable();
		int     nFKColumn = m_vColumns[k].FKColumn();

		if ( (pFKTable == NULL) || (bIsNull) )
			continue;

		ASSERT(pFKTable->SelectRow(nFKColumn, oRow[k]) != NULL);
	}
#endif
}

// Unreferenced formal parameter.
#pragma warning ( default : 4100 )

/******************************************************************************
** Method:		CheckColumn()
**
** Description:	Checks the column value is valid.
**
** Parameters:	oRow		The row being checked.
**				nColumn		The column to check.
**				oValue		The columns value.
**				bUpdate		Is an UPDATE or an INSERT?
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::CheckColumn(CRow& /*oRow*/, int /*nColumn*/, const CValue& /*oValue*/, bool /*bUpdate*/) const
{
}

/******************************************************************************
** Method:		Dump()
**
** Description:	Dump the contents of the table to the stream as text.
**
** Parameters:	rStream		The stream to dump into.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTable::Dump(WCL::IOutputStream& rStream) const
{
	TArray<int>	aiColWidths;
	CString		strColList;
	int			nRowWidth = 0;

	// Get the column widths and name list.
	for (int i = 0; i < m_vColumns.Count(); i++)
	{
		CColumn& oColumn = m_vColumns[i];

		// Get the column value width and name.
		int     nWidth   = oColumn.DisplayWidth(true);
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
	for (int r = 0; r < RowCount(); r++)
	{
		CRow& oRow = m_vRows[r];

		// For all columns in the row.
		for (int c = 0; c < m_vColumns.Count(); c++)
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
