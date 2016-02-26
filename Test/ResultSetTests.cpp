////////////////////////////////////////////////////////////////////////////////
//! \file   ResultSetTests.cpp
//! \brief  The unit tests for the ResultSet class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <MDBL/ResultSet.hpp>
#include <MDBL/MDB.hpp>
#include <MDBL/Table.hpp>

namespace
{

static void createRows(CTable& table, size_t count)
{
	for (size_t i = 0; i != count; ++i)
		table.InsertRow(table.CreateRow(true));
}

}

TEST_SET(ResultSet)
{
	CMDB   mdb;

TEST_CASE("a result set can be sorted by a nullable integer column in ascending order")
{
	CTable table(mdb, TXT("Test"));
	table.AddColumn(TXT("Value"), MDCT_INT, 0, CColumn::NULLABLE);
	createRows(table, 5);

	table[0][0] = 1;
	table[1][0] = 100;
	table[2][0] = null;
	table[3][0] = 10;
	table[4][0] = 100;

	CResultSet rows = table.SelectAll();
	rows.OrderBy(0, CSortColumns::ASC);

	TEST_TRUE(rows.Count() == 5);
	TEST_TRUE(rows[0][0] == null);
	TEST_TRUE(rows[1][0] == 1);
	TEST_TRUE(rows[2][0] == 10);
	TEST_TRUE(rows[3][0] == 100);
	TEST_TRUE(rows[4][0] == 100);
}
TEST_CASE_END

TEST_CASE("a result set can be sorted by a nullable integer column in descending order")
{
	CTable table(mdb, TXT("Test"));
	table.AddColumn(TXT("Value"), MDCT_INT, 0, CColumn::NULLABLE);
	createRows(table, 5);

	table[0][0] = 1;
	table[1][0] = 100;
	table[2][0] = null;
	table[3][0] = 10;
	table[4][0] = 100;

	CResultSet rows = table.SelectAll();
	rows.OrderBy(0, CSortColumns::DESC);

	TEST_TRUE(rows.Count() == 5);
	TEST_TRUE(rows[0][0] == 100);
	TEST_TRUE(rows[1][0] == 100);
	TEST_TRUE(rows[2][0] == 10);
	TEST_TRUE(rows[3][0] == 1);
	TEST_TRUE(rows[4][0] == null);
}
TEST_CASE_END

TEST_CASE("a result set can be sorted by a nullable, case-sensitive string column in ascending order")
{
	CTable table(mdb, TXT("Test"));
	table.AddColumn(TXT("Value"), MDCT_VARSTR, 50, CColumn::NULLABLE | CColumn::COMPARE_CASE);
	createRows(table, 5);

	table[0][0] = TXT("ABC");
	table[1][0] = TXT("xyz");
	table[2][0] = null;
	table[3][0] = TXT("XYZ");
	table[4][0] = TXT("xyz");

	CResultSet rows = table.SelectAll();
	rows.OrderBy(0, CSortColumns::ASC);

	TEST_TRUE(rows.Count() == 5);
	TEST_TRUE(rows[0][0] == null);
	TEST_TRUE(rows[1][0].GetString() == tstring(TXT("ABC")));
	TEST_TRUE(rows[2][0].GetString() == tstring(TXT("XYZ")));
	TEST_TRUE(rows[3][0].GetString() == tstring(TXT("xyz")));
	TEST_TRUE(rows[4][0].GetString() == tstring(TXT("xyz")));
}
TEST_CASE_END

TEST_CASE("a result set can be sorted by a nullable, case-sensitive string column in descending order")
{
	CTable table(mdb, TXT("Test"));
	table.AddColumn(TXT("Value"), MDCT_VARSTR, 50, CColumn::NULLABLE | CColumn::COMPARE_CASE);
	createRows(table, 5);

	table[0][0] = TXT("ABC");
	table[1][0] = TXT("xyz");
	table[2][0] = null;
	table[3][0] = TXT("XYZ");
	table[4][0] = TXT("xyz");

	CResultSet rows = table.SelectAll();
	rows.OrderBy(0, CSortColumns::DESC);

	TEST_TRUE(rows.Count() == 5);
	TEST_TRUE(rows[0][0].GetString() == tstring(TXT("xyz")));
	TEST_TRUE(rows[1][0].GetString() == tstring(TXT("xyz")));
	TEST_TRUE(rows[2][0].GetString() == tstring(TXT("XYZ")));
	TEST_TRUE(rows[3][0].GetString() == tstring(TXT("ABC")));
	TEST_TRUE(rows[4][0] == null);
}
TEST_CASE_END

TEST_CASE("a result set can be sorted by a nullable, case-insensitive string column in ascending order")
{
	CTable table(mdb, TXT("Test"));
	table.AddColumn(TXT("Value"), MDCT_VARSTR, 50, CColumn::NULLABLE | CColumn::IGNORE_CASE);
	createRows(table, 5);

	table[0][0] = TXT("ABC");
	table[1][0] = TXT("xyz");
	table[2][0] = null;
	table[3][0] = TXT("abc");
	table[4][0] = TXT("XYZ");

	CResultSet rows = table.SelectAll();
	rows.OrderBy(0, CSortColumns::ASC);

	TEST_TRUE(rows.Count() == 5);
	TEST_TRUE(rows[0][0] == null);
	TEST_TRUE(rows[1][0].GetString() == tstring(TXT("ABC")) || rows[1][0].GetString() == tstring(TXT("abc")));
	TEST_TRUE(rows[2][0].GetString() == tstring(TXT("ABC")) || rows[2][0].GetString() == tstring(TXT("abc")));
	TEST_TRUE(rows[3][0].GetString() == tstring(TXT("XYZ")) || rows[3][0].GetString() == tstring(TXT("xyz")));
	TEST_TRUE(rows[4][0].GetString() == tstring(TXT("XYZ")) || rows[4][0].GetString() == tstring(TXT("xyz")));
}
TEST_CASE_END

TEST_CASE("a result set can be sorted by a nullable, case-insensitive string column in descending order")
{
	CTable table(mdb, TXT("Test"));
	table.AddColumn(TXT("Value"), MDCT_VARSTR, 50, CColumn::NULLABLE | CColumn::IGNORE_CASE);
	createRows(table, 5);

	table[0][0] = TXT("ABC");
	table[1][0] = TXT("xyz");
	table[2][0] = null;
	table[3][0] = TXT("abc");
	table[4][0] = TXT("XYZ");

	CResultSet rows = table.SelectAll();
	rows.OrderBy(0, CSortColumns::DESC);

	TEST_TRUE(rows.Count() == 5);
	TEST_TRUE(rows[0][0].GetString() == tstring(TXT("XYZ")) || rows[0][0].GetString() == tstring(TXT("xyz")));
	TEST_TRUE(rows[1][0].GetString() == tstring(TXT("XYZ")) || rows[1][0].GetString() == tstring(TXT("xyz")));
	TEST_TRUE(rows[2][0].GetString() == tstring(TXT("ABC")) || rows[2][0].GetString() == tstring(TXT("abc")));
	TEST_TRUE(rows[3][0].GetString() == tstring(TXT("ABC")) || rows[3][0].GetString() == tstring(TXT("abc")));
	TEST_TRUE(rows[4][0] == null);
}
TEST_CASE_END

}
TEST_SET_END
