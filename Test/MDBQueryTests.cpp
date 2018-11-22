////////////////////////////////////////////////////////////////////////////////
//! \file   MDBQueryTests.cpp
//! \brief  The unit tests for the MDBQueryTests class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <MDBL/MDB.hpp>
#include <MDBL/Table.hpp>
#include <MDBL/Join.hpp>
#include <MDBL/JoinedSet.hpp>
#include <Core/UniquePtr.hpp>

TEST_SET(MDBQueryTests)
{

TEST_CASE("A query with an inner join only returns rows where the values are equal in both rows")
{
	CTable table1(TXT("1st-Table"));
	table1.AddColumn(TXT("1st"), MDCT_INT, 0);
	table1.AddColumn(TXT("2nd"), MDCT_VARSTR, 256);

	{ CRow& row = table1.CreateRow(); row[0] = 10; row[1] = TXT("N"); table1.InsertRow(row); }
	{ CRow& row = table1.CreateRow(); row[0] = 20; row[1] = TXT("Y"); table1.InsertRow(row); }
	{ CRow& row = table1.CreateRow(); row[0] = 20; row[1] = TXT("Y"); table1.InsertRow(row); }

	CTable table2(TXT("2nd-Table"));
	table2.AddColumn(TXT("1st"), MDCT_VARSTR, 256);
	table2.AddColumn(TXT("2nd"), MDCT_INT, 0);

	{ CRow& row = table2.CreateRow(); row[0] = TXT("Y"); row[1] = 20; table2.InsertRow(row); }
	{ CRow& row = table2.CreateRow(); row[0] = TXT("Y"); row[1] = 20; table2.InsertRow(row); }
	{ CRow& row = table2.CreateRow(); row[0] = TXT("N"); row[1] = 30; table2.InsertRow(row); }

	CMDB mdb;
	mdb.AddTable(table1);
	mdb.AddTable(table2);

	const size_t firstTable = 0;
	const size_t secondTable = 1;
	const size_t lhsColumn = 0;
	const size_t rhsColumn = 1;

	CJoin join(firstTable);
	join.Add(secondTable, lhsColumn, INNER_JOIN, rhsColumn);	

	CJoinedSet results = mdb.Select(join);

	TEST_TRUE(results.Count() == 4);

	TEST_TRUE(results[firstTable][0][lhsColumn] == 20 && results[secondTable][0][rhsColumn] == 20);
	TEST_TRUE(results[firstTable][1][lhsColumn] == 20 && results[secondTable][1][rhsColumn] == 20);
	TEST_TRUE(results[firstTable][2][lhsColumn] == 20 && results[secondTable][2][rhsColumn] == 20);
	TEST_TRUE(results[firstTable][3][lhsColumn] == 20 && results[secondTable][3][rhsColumn] == 20);

	TEST_TRUE(results[firstTable][0][1] == TXT("Y") && results[secondTable][0][0] == TXT("Y"));
	TEST_TRUE(results[firstTable][1][1] == TXT("Y") && results[secondTable][1][0] == TXT("Y"));
	TEST_TRUE(results[firstTable][2][1] == TXT("Y") && results[secondTable][2][0] == TXT("Y"));
	TEST_TRUE(results[firstTable][3][1] == TXT("Y") && results[secondTable][3][0] == TXT("Y"));
}
TEST_CASE_END

TEST_CASE("A query with an outer join returns an empty row on the right table where the left table has no matches")
{
	CTable table1(TXT("1st-Table"));
	table1.AddColumn(TXT("1st"), MDCT_INT, 0);
	table1.AddColumn(TXT("2nd"), MDCT_VARSTR, 256);

	{ CRow& row = table1.CreateRow(); row[0] = 10; row[1] = TXT("N"); table1.InsertRow(row); }
	{ CRow& row = table1.CreateRow(); row[0] = 20; row[1] = TXT("Y"); table1.InsertRow(row); }
	{ CRow& row = table1.CreateRow(); row[0] = 20; row[1] = TXT("Y"); table1.InsertRow(row); }

	CTable table2(TXT("2nd-Table"));
	table2.AddColumn(TXT("1st"), MDCT_VARSTR, 256);
	table2.AddColumn(TXT("2nd"), MDCT_INT, 0);

	{ CRow& row = table2.CreateRow(); row[0] = TXT("Y"); row[1] = 20; table2.InsertRow(row); }
	{ CRow& row = table2.CreateRow(); row[0] = TXT("Y"); row[1] = 20; table2.InsertRow(row); }
	{ CRow& row = table2.CreateRow(); row[0] = TXT("N"); row[1] = 30; table2.InsertRow(row); }

	CMDB mdb;
	mdb.AddTable(table1);
	mdb.AddTable(table2);

	const size_t firstTable = 0;
	const size_t secondTable = 1;
	const size_t lhsColumn = 0;
	const size_t rhsColumn = 1;

	CJoin join(firstTable);
	join.Add(secondTable, lhsColumn, OUTER_JOIN, rhsColumn);	

	CJoinedSet results = mdb.Select(join);

	TEST_TRUE(results.Count() == 5);

	TEST_TRUE(results[firstTable][0][lhsColumn] == 10 && results[secondTable][0][rhsColumn] == null);
	TEST_TRUE(results[firstTable][1][lhsColumn] == 20 && results[secondTable][1][rhsColumn] == 20);
	TEST_TRUE(results[firstTable][2][lhsColumn] == 20 && results[secondTable][2][rhsColumn] == 20);
	TEST_TRUE(results[firstTable][3][lhsColumn] == 20 && results[secondTable][3][rhsColumn] == 20);
	TEST_TRUE(results[firstTable][4][lhsColumn] == 20 && results[secondTable][4][rhsColumn] == 20);

	TEST_TRUE(results[firstTable][0][1] == TXT("N") && results[secondTable][0][0] == null);
	TEST_TRUE(results[firstTable][1][1] == TXT("Y") && results[secondTable][1][0] == TXT("Y"));
	TEST_TRUE(results[firstTable][2][1] == TXT("Y") && results[secondTable][2][0] == TXT("Y"));
	TEST_TRUE(results[firstTable][3][1] == TXT("Y") && results[secondTable][3][0] == TXT("Y"));
	TEST_TRUE(results[firstTable][4][1] == TXT("Y") && results[secondTable][4][0] == TXT("Y"));
}
TEST_CASE_END

}
TEST_SET_END
