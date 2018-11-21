////////////////////////////////////////////////////////////////////////////////
//! \file   MDB.cpp
//! \brief  The unit tests for the MDB class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <MDBL/MDB.hpp>
#include "Mocks/MockSQLSource.hpp"
#include "Mocks/MockSQLCursor.hpp"

using namespace Mocks;

TEST_SET(MDB)
{

TEST_CASE("A table can be created from the result set of an explicit query")
{
	const tchar* anyConnection = TXT("any SQL connection");
	const tchar* anyQuery = TXT("any SQL query");

	SQLColumn column = { 0, TXT("column"), 0, MDCT_VARSTR, 0, 0, CColumn::NULLABLE };
	MockSQLCursor::Columns columns;
	columns.push_back(column);

	MockSQLCursorPtr mockCursor(new MockSQLCursor);
	mockCursor->SetColumns(columns);

	MockSQLSource mockSource;
	mockSource.Open(anyConnection);
	mockSource.SetCursor(mockCursor);

	CMDB mdb;
	CTable::Ptr table = mdb.CreateTable(TXT("table"), mockSource, anyQuery);

	TEST_TRUE(table->Name() == TXT("table"));
	TEST_TRUE(table->ColumnCount() == 1);
	TEST_TRUE(table->Column(0).Name() == TXT("column"));
	TEST_TRUE(table->RowCount() == 0);
}
TEST_CASE_END

}
TEST_SET_END
