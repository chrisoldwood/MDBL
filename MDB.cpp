/******************************************************************************
** (C) Chris Oldwood
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
			pTable->InsertRow(oRow);
			oRow.Status(CRow::ORIGINAL);
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
	for (int i = 0; i < m_vTables.Size(); i++)
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
** Parameters:	oJoin	The join query.
**
** Returns:		The result set.
**
*******************************************************************************
*/

CJoinedSet CMDB::Select(const CJoin& oJoin) const
{
	return CJoinedSet(0);
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
	for (int i = 0; i < m_vTables.Size(); i++)
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
	for (int i = 0; i < m_vTables.Size(); i++)
		m_vTables[i] << rStream;
}

void CMDB::operator >>(CStream& rStream)
{
	// For all tables.
	for (int i = 0; i < m_vTables.Size(); i++)
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
	for (int i = 0; i < m_vTables.Size(); i++)
		m_vTables[i] << rSource;
}

void CMDB::operator >>(CSQLSource& rSource)
{
	// For all tables.
	for (int i = 0; i < m_vTables.Size(); i++)
		m_vTables[i] >> rSource;
}
