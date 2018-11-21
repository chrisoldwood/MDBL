/******************************************************************************
**
** MODULE:		MDB.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CMDB class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "MDB.hpp"
#include "SQLSource.hpp"
#include "SQLCursor.hpp"
#include "SQLException.hpp"
#include "JoinedSet.hpp"
#include "Join.hpp"
#include <malloc.h>
#include <Core/UniquePtr.hpp>

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
	: m_vTables()
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

CTable::Ptr CMDB::CreateTable(const tchar* pszName)
{
	ASSERT(pszName != nullptr);

	return CTable::Ptr(new CTable(pszName));
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

CTable::Ptr CMDB::CreateTable(const tchar* pszName, CSQLSource& oConnection, const tchar* pszQuery)
{
	ASSERT(pszName != nullptr);
	ASSERT(oConnection.IsOpen());

	CString strQuery;

	// Only table name specified?
	if (pszQuery == nullptr)
	{
		strQuery.Format(TXT("SELECT * FROM %s"), pszName);
		pszQuery = strQuery;
	}

	return CTable::Create(pszName, oConnection, pszQuery);
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

size_t CMDB::AddTable(CTable& oTable)
{
	ASSERT(FindTable(oTable.Name()) == Core::npos);

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

size_t CMDB::FindTable(const tchar* pszName)
{
	// For all tables.
	for (size_t i = 0; i < m_vTables.Count(); ++i)
	{
		if (m_vTables[i].Name() == pszName)
			return i;
	}

	return Core::npos;
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
	ASSERT(oQuery.Count() > 1);

	size_t nJoins = oQuery.Count();

	// Create the table list.
	CTable** apTables = static_cast<CTable**>(_alloca(sizeof(CTable*) * nJoins));

	for (size_t i = 0; i != nJoins; ++i)
		apTables[i] = &Table(oQuery[i].m_nTable);;

	// Create the joined result set.
	CJoinedSet oJS(nJoins, apTables);

	// Run the query.
	DoJoin(oQuery, 0, *(static_cast<CRow*>(nullptr)), oJS);

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

size_t CMDB::DoJoin(const CJoin& oQuery, size_t nJoin, const CRow& oLHSRow, CJoinedSet& oJS) const
{
	// Get the table to join on.
	CTable& oRHSTable = Table(oQuery[nJoin].m_nTable);

	// Get the columns to join on.
	size_t nLHSColumn = oQuery[nJoin].m_nLHSColumn;
	size_t nRHSColumn = oQuery[nJoin].m_nRHSColumn;

	size_t nMatches = 0;

	// For all rows in the table.
	for (size_t r = 0; r < oRHSTable.RowCount(); ++r)
	{
		// Get the row.
		CRow& oRHSRow = oRHSTable[r];

		// Scanning first table OR this row matches?
		if ( (nJoin == 0) || (oRHSRow[nRHSColumn] == oLHSRow[nLHSColumn]) )
		{
			size_t nRows = 1;

			// More joins to process?
			if (nJoin < (oQuery.Count()-1))
				nRows = DoJoin(oQuery, nJoin+1, oRHSRow, oJS);

			// Join succesful?
			if (nRows > 0)
			{
				CResultSet& oRS = oJS[nJoin];

				// Add this row 'nRows' times.
				for (size_t i = 0; i < nRows; ++i)
					oRS.Add(oRHSRow);

				nMatches += nRows;
			}
			// Join failed, but OUTER join requested?
			else if (oQuery[nJoin+1].m_nType == CJoin::OUTER)
			{
				// Add this row.
				oJS[nJoin].Add(oRHSRow);

				// Add NULL row to all joins from here down.
				for (size_t i = nJoin+1; i < oQuery.Count(); ++i)
					oJS[i].Add(Table(oQuery[i].m_nTable).NullRow());

				++nMatches;
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
	for (size_t i = 0; i < m_vTables.Count(); ++i)
	{
		if (m_vTables[i].Modified())
			return true;
	}

	return false;
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

void CMDB::Read(WCL::IInputStream& rStream)
{
	// For all tables.
	for (size_t i = 0; i < m_vTables.Count(); ++i)
		m_vTables[i].Read(rStream);
}

void CMDB::Write(WCL::IOutputStream& rStream)
{
	// For all tables.
	for (size_t i = 0; i < m_vTables.Count(); ++i)
		m_vTables[i].Write(rStream);
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

void CMDB::Read(CSQLSource& rSource)
{
	ASSERT(rSource.IsOpen());

	// For all tables.
	for (size_t i = 0; i < m_vTables.Count(); ++i)
		m_vTables[i].Read(rSource);
}

void CMDB::Write(CSQLSource& rSource, CTable::RowTypes eRows)
{
	ASSERT(rSource.IsOpen());

	// Write inserted rows?
	if (eRows & CTable::INSERTED)
	{
		// Process insertions from table 1 to n.
		for (size_t i = 0; i < m_vTables.Count(); ++i)
			m_vTables[i].Write(rSource, CTable::INSERTED);
	}

	// Write updated rows?
	if (eRows & CTable::UPDATED)
	{
		// Process updates from table 1 to n.
		for (size_t i = 0; i < m_vTables.Count(); ++i)
			m_vTables[i].Write(rSource, CTable::UPDATED);
	}

	// Write deleted rows?
	if (eRows & CTable::DELETED)
	{
		// Process deletions from table n to 1.
		for (size_t i = m_vTables.Count(); i != 0; --i)
			m_vTables[i-1].Write(rSource, CTable::DELETED);
	}
}

/******************************************************************************
** Method:		ResetRowFlags()
**
** Description:	Resets the modified flag on all rows for all tables.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CMDB::ResetRowFlags()
{
	// For all tables.
	for (size_t i = 0; i < m_vTables.Count(); ++i)
		m_vTables[i].ResetRowFlags();
}

/******************************************************************************
** Method:		Dump()
**
** Description:	Dump the contents of all tables to the stream in text format.
**
** Parameters:	rStream		The stream to dump into.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CMDB::Dump(WCL::IOutputStream& rStream) const
{
	// For all tables.
	for (size_t i = 0; i < m_vTables.Count(); ++i)
		m_vTables[i].Dump(rStream);
}
