////////////////////////////////////////////////////////////////////////////////
//! \file   ResultSetTests.cpp
//! \brief  The unit tests for the ResultSet class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <MDBL/ResultSet.hpp>
#include <MDBL/MDB.hpp>
#include <MDBL/Table.hpp>

TEST_SET(ResultSet)
{
	enum Columns
	{
		INT_VALUE = 0,
		STR_VALUE = 1,
	};

	CMDB   mdb;
	CTable table(mdb, TXT("Test"));

	table.AddColumn(TXT("IntValue"), MDCT_INT,     0, CColumn::NULLABLE);
	table.AddColumn(TXT("StrValue"), MDCT_VARSTR, 50, CColumn::NULLABLE);
{
	// low values.
	CRow& row = table.CreateRow(false);
	row[INT_VALUE] = 1;
	row[STR_VALUE] = TXT("ABC");
	table.InsertRow(row);
}{
	// high values.
	CRow& row = table.CreateRow(false);
	row[INT_VALUE] = 100;
	row[STR_VALUE] = TXT("xyz");
	table.InsertRow(row);
}{
	// null values.
	CRow& row = table.CreateRow(false);
	row[INT_VALUE] = null;
	row[STR_VALUE] = null;
	table.InsertRow(row);
}{
	// in-between values.
	CRow& row = table.CreateRow(false);
	row[INT_VALUE] = 10;
	row[STR_VALUE] = TXT("XYZ");
	table.InsertRow(row);
}{
	// duplicate values.
	CRow& row = table.CreateRow(false);
	row[INT_VALUE] = 100;
	row[STR_VALUE] = TXT("xyz");
	table.InsertRow(row);
}

TEST_CASE("a result set can be sorted by a nullable integer column in ascending order")
{
	CResultSet rows = table.SelectAll();

	rows.OrderBy(CSortColumns(INT_VALUE, CSortColumns::ASC));

	TEST_TRUE(rows.Count() == 5);
	TEST_TRUE(rows[0][INT_VALUE] == null);
	TEST_TRUE(rows[1][INT_VALUE] == 1);
	TEST_TRUE(rows[2][INT_VALUE] == 10);
	TEST_TRUE(rows[3][INT_VALUE] == 100);
	TEST_TRUE(rows[4][INT_VALUE] == 100);
}
TEST_CASE_END

TEST_CASE("a result set can be sorted by a nullable integer column in descending order")
{
	CResultSet rows = table.SelectAll();

	rows.OrderBy(CSortColumns(INT_VALUE, CSortColumns::DESC));

	TEST_TRUE(rows.Count() == 5);
	TEST_TRUE(rows[0][INT_VALUE] == 100);
	TEST_TRUE(rows[1][INT_VALUE] == 100);
	TEST_TRUE(rows[2][INT_VALUE] == 10);
	TEST_TRUE(rows[3][INT_VALUE] == 1);
	TEST_TRUE(rows[4][INT_VALUE] == null);
}
TEST_CASE_END

TEST_CASE("a result set can be sorted by a nullable string column in ascending order")
{
	CResultSet rows = table.SelectAll();

	rows.OrderBy(CSortColumns(STR_VALUE, CSortColumns::ASC));

	TEST_TRUE(rows.Count() == 5);
	TEST_TRUE(rows[0][STR_VALUE] == null);
	TEST_TRUE(rows[1][STR_VALUE].GetString() == tstring(TXT("ABC")));
	TEST_TRUE(rows[2][STR_VALUE].GetString() == tstring(TXT("XYZ")));
	TEST_TRUE(rows[3][STR_VALUE].GetString() == tstring(TXT("xyz")));
	TEST_TRUE(rows[4][STR_VALUE].GetString() == tstring(TXT("xyz")));
}
TEST_CASE_END

TEST_CASE("a result set can be sorted by a nullable string column in descending order")
{
	CResultSet rows = table.SelectAll();

	rows.OrderBy(CSortColumns(STR_VALUE, CSortColumns::DESC));

	TEST_TRUE(rows.Count() == 5);
	TEST_TRUE(rows[0][STR_VALUE].GetString() == tstring(TXT("xyz")));
	TEST_TRUE(rows[1][STR_VALUE].GetString() == tstring(TXT("xyz")));
	TEST_TRUE(rows[2][STR_VALUE].GetString() == tstring(TXT("XYZ")));
	TEST_TRUE(rows[3][STR_VALUE].GetString() == tstring(TXT("ABC")));
	TEST_TRUE(rows[4][STR_VALUE] == null);
}
TEST_CASE_END

}
TEST_SET_END
