////////////////////////////////////////////////////////////////////////////////
//! \file   TableTests.cpp
//! \brief  The unit tests for the Table class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <MDBL/Table.hpp>
#include <WCL/Path.hpp>
#include <MDBL/ODBCSource.hpp>
#include <MDBL/TimeStamp.hpp>
#include "Mocks/MockSQLSource.hpp"
#include "Mocks/MockSQLCursor.hpp"

using namespace Mocks;

TEST_SET(Table)
{
	const CPath testRunnerFolder = CPath::ApplicationDir();
	const CPath databaseFolder = testRunnerFolder.Parent() / TXT("Database");

	ASSERT(databaseFolder.Exists());

	tstring connection = TXT("Driver={Microsoft Text Driver (*.txt; *.csv)};");
	connection += Core::fmt(TXT("Dbq=%s;"), databaseFolder.c_str());

	CODBCSource source(connection);

TEST_CASE("A table can be populated by executing an implicit query against a SQL data source")
{
	CTable table(TXT("TestValues.csv"));

	table.AddColumn(TXT("BitValue"),       MDCT_BOOL,        0, CColumn::NULLABLE);
	table.AddColumn(TXT("ByteValue"),      MDCT_INT,         0, CColumn::NULLABLE);
	table.AddColumn(TXT("ShortValue"),     MDCT_INT,         0, CColumn::NULLABLE);
	table.AddColumn(TXT("LongValue"),      MDCT_INT,         0, CColumn::NULLABLE);
	table.AddColumn(TXT("SingleValue"),    MDCT_DOUBLE,      0, CColumn::NULLABLE);
	table.AddColumn(TXT("DoubleValue"),    MDCT_DOUBLE,      0, CColumn::NULLABLE);
	table.AddColumn(TXT("DateTimeValue"),  MDCT_TIMESTAMP,   0, CColumn::NULLABLE);
	table.AddColumn(TXT("ShortTextValue"), MDCT_FXDSTR,     42, CColumn::NULLABLE);
	table.AddColumn(TXT("LongTextValue"),  MDCT_VARSTR,   1024, CColumn::NULLABLE);

	table.Read(source);

	TEST_TRUE(table.RowCount() == 1);

	const CRow& row = table[0];

	TEST_TRUE(row[0].GetBool() == true);
	TEST_TRUE(row[1].GetInt() == 2);
	TEST_TRUE(row[2].GetInt() == 3);
	TEST_TRUE(row[3].GetInt() == 4);
	TEST_TRUE(row[4].GetDouble() == 5.0);
	TEST_TRUE(row[5].GetDouble() == 6.0);
	TEST_TRUE(row[6].GetTimeStamp() == CTimeStamp(2006, 5, 4, 1, 2, 3));
	TEST_TRUE(row[7].GetString() == tstring(TXT("short text")));
	TEST_TRUE(row[8].GetString() == tstring(TXT("long text")));
}
TEST_CASE_END

TEST_CASE("When populating a table from an implicit query transient columns should be skipped")
{
	CTable table(TXT("TestValues.csv"));

	table.AddColumn(TXT("TransientColumn"), MDCT_INT,     0, CColumn::TRANSIENT);
	table.AddColumn(TXT("ShortTextValue"),  MDCT_FXDSTR, 42, CColumn::NULLABLE);

	table.Read(source);

	TEST_TRUE(table.RowCount() == 1);

	const CRow& row = table[0];

	TEST_TRUE(row[1].GetString() == tstring(TXT("short text")));
}
TEST_CASE_END

TEST_CASE("A table can be created from the result set of an explicit query")
{
	const tchar* anyConnection = TXT("any SQL connection");
	const tchar* anyQuery = TXT("any SQL query");

	MockSQLColumn column(0, TXT("column"), MDCT_VARSTR, 99, CColumn::NULLABLE);
	MockSQLCursor::Columns columns;
	columns.push_back(column);

	MockSQLCursorPtr mockCursor(new MockSQLCursor);
	mockCursor->SetColumns(columns);

	MockSQLSource mockSource;
	mockSource.Open(anyConnection);
	mockSource.SetCursor(mockCursor);

	CTable::Ptr table = CTable::Create(mockSource, anyQuery);

	TEST_TRUE(table->Name() == TXT(""));
	TEST_TRUE(table->ColumnCount() == 1);
	TEST_TRUE(table->Column(0).Name() == TXT("column"));
	TEST_TRUE(table->RowCount() == 0);
}
TEST_CASE_END

}
TEST_SET_END
