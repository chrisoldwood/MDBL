/******************************************************************************
**
** MODULE:		MDB.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CMDB class definition.
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

CMDB::CMDB()
{
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

CMDB::~CMDB()
{
}

/******************************************************************************
** Method:		CreateTable()
**
** Description:	Creates a new table from the given name.
**
** Parameters:	pszName		The name ofthe new table.
**
** Returns:		The new table.
**
*******************************************************************************
*/

CTable& CMDB::CreateTable(const char* pszName)
{
	ASSERT(pszName != NULL);

	return *(new CTable(*this, pszName));
}

/******************************************************************************
** Method:		CreateTable()
**
** Description:	Creates a new table using the given connection and query.
**
** Parameters:	pszName			The tables name.
**				oConnection		The SQL connection.
**				pszQuery		The query to use to fill the table.
**
** Returns:		The new table.
**
*******************************************************************************
*/

CTable& CMDB::CreateTable(const char* pszName, CSQLSource& oConnection, const char* pszQuery)
{
	ASSERT(pszName != NULL);
	ASSERT(oConnection.IsOpen());

	CString strQuery;

	// Only table name specified?
	if (pszQuery == NULL)
	{
		strQuery.Format("SELECT * FROM %s", pszName);
		pszQuery = strQuery;
	}

	CTable*		pTable  = new CTable(*this, pszName);
	CSQLCursor*	pCursor = NULL;

	try
	{
		pCursor = oConnection.ExecQuery(pszQuery);

		// Setup the tables' schema.
		for (int i = 0; i < pCursor->NumColumns(); i++)
		{
			SQLColumn& oColumn = pCursor->Column(i);

			pTable->AddColumn(oColumn.m_strName, oColumn.m_eMDBColType, oColumn.m_nSize, oColumn.m_nFlags);
		}

		// For all rows.
		while (pCursor->Fetch())
		{
			// Allocate the row.
			CRow& oRow = pTable->CreateRow();

			// Copy the data.
			pCursor->SetRow(oRow);

			// Append to table.
			pTable->InsertRow(oRow, false);
		}

		// Cleanup.
		delete pCursor;
	}
	catch(CSQLException&)
	{
		delete pCursor;
		delete pTable;
		throw;
	}

	return *pTable;
}

/******************************************************************************
** Method:		AddTable()
**
** Description:	Adds a table to the database.
**
** Parameters:	oTable	The table to add.
**
** Returns:		The index of the new table.
**
*******************************************************************************
*/

int CMDB::AddTable(CTable& oTable)
{
	ASSERT(FindTable(oTable.Name()) == -1);

	return m_vTables.Add(oTable);
}

/******************************************************************************
** Method:		FindTable()
**
** Description:	Finds a table given the name.
**
** Parameters:	None.
**
** Returns:		The index or -1 if not found.
**
*******************************************************************************
*/

int CMDB::FindTable(const char* pszName)
{
	// For all tables.
	for (int i = 0; i < m_vTables.Count(); i++)
	{
		if (m_vTables[i].Name() == pszName)
			return i;
	}

	return -1;
}

/******************************************************************************
** Method:		Select()
**
** Description:	Executes a query involving a join across tables.
**
** Parameters:	oQuery	The join query.
**
** Returns:		The result set.
**
*******************************************************************************
*/

CJoinedSet CMDB::Select(const CJoin& oQuery) const
{
	// Create the joined result set.
	CJoinedSet oJS(oQuery.Count());

	ASSERT(oQuery.Count() > 1);

	// Run the query.
	DoJoin(oQuery, 0, *((CRow*)(NULL)), oJS);

	return oJS;
}

/******************************************************************************
** Method:		DoJoin()
**
** Description:	Internal method to perform a single join.
**
** Parameters:	oQuery	The join query.
**				nJoin	The join to perform.
**				oJS		The result set to append to.
**
** Returns:		The number of rows appended.
**
*******************************************************************************
*/

int CMDB::DoJoin(const CJoin& oQuery, int nJoin, const CRow& oLHSRow, CJoinedSet& oJS) const
{
	// Get the table to join on.
	CTable& oRHSTable = Table(oQuery[nJoin].m_nTable);

	// Get the columns to join on.
	int nLHSColumn = oQuery[nJoin].m_nLHSColumn;
	int nRHSColumn = oQuery[nJoin].m_nRHSColumn;

	int nMatches = 0;

	// For all rows in the table.
	for (int r = 0; r < oRHSTable.RowCount(); r++)
	{
		// Get the row.
		CRow& oRHSRow = oRHSTable[r];

		// Scanning first table OR this row matches?
		if ( (nJoin == 0) || (oRHSRow[nRHSColumn] == oLHSRow[nLHSColumn]) )
		{
			int nRows = 1;

			// More joins to process?
			if (nJoin < (oQuery.Count()-1))
				nRows = DoJoin(oQuery, nJoin+1, oRHSRow, oJS);

			// Join succesful?
			if (nRows > 0)
			{
				CResultSet& oRS = oJS[nJoin];

				// Add this row 'nRows' times.
				for (int i = 0; i < nRows; i++)
					oRS.Add(oRHSRow);

				nMatches += nRows;
			}
			// Join failed, but OUTER join requested?
			else if (oQuery[nJoin+1].m_nType == CJoin::OUTER)
			{
				// Add this row.
				oJS[nJoin].Add(oRHSRow);

				// Add NULL row to all joins from here down.
				for (int i = nJoin+1; i < oQuery.Count(); i++)
					oJS[i].Add(Table(oQuery[i].m_nTable).NullRow());

				nMatches++;
			}
		}
	}

	return nMatches;
}

/******************************************************************************
** Method:		Modified()
**
** Description:	Checks if any changes have been made to any tables.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CMDB::Modified() const
{
	// For all tables.
	for (int i = 0; i < m_vTables.Count(); i++)
	{
		if (m_vTables[i].Modified())
			return true;
	}

	return false;
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

void CMDB::operator <<(CStream& rStream)
{
	// For all tables.
	for (int i = 0; i < m_vTables.Count(); i++)
		m_vTables[i] << rStream;
}

void CMDB::operator >>(CStream& rStream)
{
	// For all tables.
	for (int i = 0; i < m_vTables.Count(); i++)
		m_vTables[i] >> rStream;
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

void CMDB::operator <<(CSQLSource& rSource)
{
	// For all tables.
	for (int i = 0; i < m_vTables.Count(); i++)
		m_vTables[i] << rSource;
}

void CMDB::operator >>(CSQLSource& rSource)
{
	// For all tables.
	for (int i = 0; i < m_vTables.Count(); i++)
		m_vTables[i] >> rSource;
}
