////////////////////////////////////////////////////////////////////////////////
//! \file   TableTests.cpp
//! \brief  The unit tests for the Table class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <MDBL/Table.hpp>
#include <WCL/Path.hpp>
#include <MDBL/ODBCSource.hpp>
#include <MDBL/MDB.hpp>
#include <MDBL/TimeStamp.hpp>

TEST_SET(Table)
{
	const CPath testRunnerFolder = CPath::ApplicationDir();
	const CPath databaseFolder = testRunnerFolder.Parent() / TXT("Database");

	ASSERT(databaseFolder.Exists());

	tstring connection = TXT("Driver={Microsoft Text Driver (*.txt; *.csv)};");
	connection += Core::fmt(TXT("Dbq=%s;"), databaseFolder.c_str());

	CODBCSource source(connection);

TEST_CASE("The table can be populated by executing a query against a SQL data source")
{
	CMDB mdb;
	CTable table(mdb, TXT("TestValues.csv"));

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

TEST_CASE("When populating a table from a query transient columns should be skipped")
{
	CMDB mdb;
	CTable table(mdb, TXT("TestValues.csv"));

	table.AddColumn(TXT("TransientColumn"), MDCT_INT,     0, CColumn::TRANSIENT);
	table.AddColumn(TXT("ShortTextValue"),  MDCT_FXDSTR, 42, CColumn::NULLABLE);

	table.Read(source);

	TEST_TRUE(table.RowCount() == 1);

	const CRow& row = table[0];

	TEST_TRUE(row[1].GetString() == tstring(TXT("short text")));
}
TEST_CASE_END

}
TEST_SET_END
