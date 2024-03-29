////////////////////////////////////////////////////////////////////////////////
//! \file   ODBCCursorTests.cpp
//! \brief  The integration tests for the CODBCCursor class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <MDBL/ODBCSource.hpp>
#include <MDBL/ODBCCursor.hpp>
#include <WCL/Path.hpp>
#include <MDBL/Column.hpp>
#include <MDBL/MDB.hpp>
#include <MDBL/TimeStamp.hpp>

TEST_SET(ODBCCursor)
{
	const CPath testRunnerFolder = CPath::ApplicationDir();
	const CPath databaseFolder = testRunnerFolder.Parent().Parent() / TXT("Database");

	ASSERT(databaseFolder.Exists());

#ifdef _WIN64
	tstring driver = TXT("Microsoft Access Text Driver (*.txt, *.csv)");
#else
	tstring driver = TXT("Microsoft Text Driver (*.txt; *.csv)");
#endif
	tstring connection = Core::fmt(TXT("Driver={%s};"), driver.c_str());
	connection += Core::fmt(TXT("Dbq=%s;"), databaseFolder.c_str());

	CODBCSource source(connection);

	const tstring query = TXT("SELECT * FROM TestValues.csv");

TEST_CASE("Fetching a row returns true when a row was retrieved")
{
	SQLCursorPtr cursor = source.ExecQuery(query);

	TEST_TRUE(cursor->Fetch());
}
TEST_CASE_END

TEST_CASE("Fetching a row returns false when the end of the result set is reached")
{
	SQLCursorPtr cursor = source.ExecQuery(query);

	cursor->Fetch();

	TEST_FALSE(cursor->Fetch());
}
TEST_CASE_END

TEST_CASE("The number of columns in the result set can be retrieved")
{
	SQLCursorPtr cursor = source.ExecQuery(query);

	TEST_TRUE(cursor->NumColumns() == 9);
}
TEST_CASE_END

TEST_CASE("The meta-data for each column can be retrieved")
{
	SQLCursorPtr cursor = source.ExecQuery(query);
	const size_t numColumns = cursor->NumColumns();

	for (size_t i = 0; i != numColumns; ++i)
	{
		const SQLColumn& column = cursor->Column(i);

		TEST_TRUE(column.m_nDstColumn == i);
		TEST_FALSE(column.m_strName.Empty())
	}
}
TEST_CASE_END

TEST_CASE("The SQL bit type is mapped onto the boolean column type")
{
	SQLCursorPtr cursor = source.ExecQuery(query);

	const SQLColumn& column = cursor->Column(0);

	TEST_TRUE(column.m_nDstColumn == 0);
	TEST_TRUE(column.m_strName == TXT("BitValue"));
	TEST_TRUE(column.m_eMDBColType == MDCT_BOOL);
	TEST_TRUE(column.m_nSize == 1);
	TEST_TRUE(column.m_nFlags == CColumn::NOT_NULLABLE);

	const ODBCColumn& odbcColumn = static_cast<const ODBCColumn&>(column);

	TEST_TRUE(odbcColumn.m_nSQLColType == SQL_BIT);
	TEST_TRUE(odbcColumn.m_nSQLFetchType == SQL_UNKNOWN_TYPE);
}
TEST_CASE_END

TEST_CASE("The SQL byte type is mapped onto the integer column type")
{
	SQLCursorPtr cursor = source.ExecQuery(query);

	const SQLColumn& column = cursor->Column(1);

	TEST_TRUE(column.m_nDstColumn == 1);
	TEST_TRUE(column.m_strName == TXT("ByteValue"));
	TEST_TRUE(column.m_eMDBColType == MDCT_INT);
	TEST_TRUE(column.m_nSize == 3);
	TEST_TRUE(column.m_nFlags == CColumn::NULLABLE);

	const ODBCColumn& odbcColumn = static_cast<const ODBCColumn&>(column);

	TEST_TRUE(odbcColumn.m_nSQLColType == SQL_TINYINT);
	TEST_TRUE(odbcColumn.m_nSQLFetchType == SQL_UNKNOWN_TYPE);
}
TEST_CASE_END

TEST_CASE("The SQL short type is mapped onto the integer column type")
{
	SQLCursorPtr cursor = source.ExecQuery(query);

	const SQLColumn& column = cursor->Column(2);

	TEST_TRUE(column.m_nDstColumn == 2);
	TEST_TRUE(column.m_strName == TXT("ShortValue"));
	TEST_TRUE(column.m_eMDBColType == MDCT_INT);
	TEST_TRUE(column.m_nSize == 5);
	TEST_TRUE(column.m_nFlags == CColumn::NULLABLE);

	const ODBCColumn& odbcColumn = static_cast<const ODBCColumn&>(column);

	TEST_TRUE(odbcColumn.m_nSQLColType == SQL_SMALLINT);
	TEST_TRUE(odbcColumn.m_nSQLFetchType == SQL_UNKNOWN_TYPE);
}
TEST_CASE_END

TEST_CASE("The SQL long type is mapped onto the integer column type")
{
	SQLCursorPtr cursor = source.ExecQuery(query);

	const SQLColumn& column = cursor->Column(3);

	TEST_TRUE(column.m_nDstColumn == 3);
	TEST_TRUE(column.m_strName == TXT("LongValue"));
	TEST_TRUE(column.m_eMDBColType == MDCT_INT);
	TEST_TRUE(column.m_nSize == 10);
	TEST_TRUE(column.m_nFlags == CColumn::NULLABLE);

	const ODBCColumn& odbcColumn = static_cast<const ODBCColumn&>(column);

	TEST_TRUE(odbcColumn.m_nSQLColType == SQL_INTEGER);
	TEST_TRUE(odbcColumn.m_nSQLFetchType == SQL_UNKNOWN_TYPE);
}
TEST_CASE_END

TEST_CASE("The SQL real type is mapped onto the double column type")
{
	SQLCursorPtr cursor = source.ExecQuery(query);

	const SQLColumn& column = cursor->Column(4);

	TEST_TRUE(column.m_nDstColumn == 4);
	TEST_TRUE(column.m_strName == TXT("SingleValue"));
	TEST_TRUE(column.m_eMDBColType == MDCT_DOUBLE);
	TEST_TRUE(column.m_nSize == 24);
	TEST_TRUE(column.m_nFlags == CColumn::NULLABLE);

	const ODBCColumn& odbcColumn = static_cast<const ODBCColumn&>(column);

	TEST_TRUE(odbcColumn.m_nSQLColType == SQL_REAL);
	TEST_TRUE(odbcColumn.m_nSQLFetchType == SQL_UNKNOWN_TYPE);
}
TEST_CASE_END

TEST_CASE("The SQL double type is mapped onto the double column type")
{
	SQLCursorPtr cursor = source.ExecQuery(query);

	const SQLColumn& column = cursor->Column(5);

	TEST_TRUE(column.m_nDstColumn == 5);
	TEST_TRUE(column.m_strName == TXT("DoubleValue"));
	TEST_TRUE(column.m_eMDBColType == MDCT_DOUBLE);
	TEST_TRUE(column.m_nSize == 53);
	TEST_TRUE(column.m_nFlags == CColumn::NULLABLE);

	const ODBCColumn& odbcColumn = static_cast<const ODBCColumn&>(column);

	TEST_TRUE(odbcColumn.m_nSQLColType == SQL_DOUBLE);
	TEST_TRUE(odbcColumn.m_nSQLFetchType == SQL_UNKNOWN_TYPE);
}
TEST_CASE_END

TEST_CASE("The SQL time-stamp type is mapped onto the time-stamp column type")
{
	SQLCursorPtr cursor = source.ExecQuery(query);

	const SQLColumn& column = cursor->Column(6);

	TEST_TRUE(column.m_nDstColumn == 6);
	TEST_TRUE(column.m_strName == TXT("DateTimeValue"));
	TEST_TRUE(column.m_eMDBColType == MDCT_TIMESTAMP);
	TEST_TRUE(column.m_nSize == 19);
	TEST_TRUE(column.m_nFlags == CColumn::NULLABLE);

	const ODBCColumn& odbcColumn = static_cast<const ODBCColumn&>(column);

	TEST_TRUE(odbcColumn.m_nSQLColType == SQL_TYPE_TIMESTAMP);
	TEST_TRUE(odbcColumn.m_nSQLFetchType == SQL_UNKNOWN_TYPE);
}
TEST_CASE_END

TEST_CASE("The SQL text type is mapped onto the string column type")
{
	SQLCursorPtr cursor = source.ExecQuery(query);

	const SQLColumn& column = cursor->Column(7);

	TEST_TRUE(column.m_nDstColumn == 7);
	TEST_TRUE(column.m_strName == TXT("ShortTextValue"));
	TEST_TRUE(column.m_eMDBColType == MDCT_FXDSTR);
	TEST_TRUE(column.m_nSize == 42);
	TEST_TRUE(column.m_nFlags == CColumn::NULLABLE);

	const ODBCColumn& odbcColumn = static_cast<const ODBCColumn&>(column);

	TEST_TRUE(odbcColumn.m_nSQLColType == SQL_VARCHAR);
	TEST_TRUE(odbcColumn.m_nSQLFetchType == SQL_UNKNOWN_TYPE);
}
TEST_CASE_END

TEST_CASE("The SQL variable length text type is mapped onto the string column type")
{
	SQLCursorPtr cursor = source.ExecQuery(query);

	const SQLColumn& column = cursor->Column(8);

	TEST_TRUE(column.m_nDstColumn == 8);
	TEST_TRUE(column.m_strName == TXT("LongTextValue"));
	TEST_TRUE(column.m_eMDBColType == MDCT_VARSTR);
	TEST_TRUE(column.m_nSize == 64000);
	TEST_TRUE(column.m_nFlags == CColumn::NULLABLE);

	const ODBCColumn& odbcColumn = static_cast<const ODBCColumn&>(column);

	TEST_TRUE(odbcColumn.m_nSQLColType == SQL_LONGVARCHAR);
	TEST_TRUE(odbcColumn.m_nSQLFetchType == SQL_UNKNOWN_TYPE);
}
TEST_CASE_END

TEST_CASE("Once fetched the values for each column in the row can be retrieved")
{
	CTable table(TXT("test"));

	table.AddColumn(TXT("BitValue"),       MDCT_BOOL,        0, CColumn::NULLABLE);
	table.AddColumn(TXT("ByteValue"),      MDCT_INT,         0, CColumn::NULLABLE);
	table.AddColumn(TXT("ShortValue"),     MDCT_INT,         0, CColumn::NULLABLE);
	table.AddColumn(TXT("LongValue"),      MDCT_INT,         0, CColumn::NULLABLE);
	table.AddColumn(TXT("SingleValue"),    MDCT_DOUBLE,      0, CColumn::NULLABLE);
	table.AddColumn(TXT("DoubleValue"),    MDCT_DOUBLE,      0, CColumn::NULLABLE);
	table.AddColumn(TXT("DateTimeValue"),  MDCT_TIMESTAMP,   0, CColumn::NULLABLE);
	table.AddColumn(TXT("ShortTextValue"), MDCT_FXDSTR,     42, CColumn::NULLABLE);
	table.AddColumn(TXT("LongTextValue"),  MDCT_VARSTR,   1024, CColumn::NULLABLE);

	SQLCursorPtr cursor = source.ExecQuery(query);
	cursor->Fetch();

	CRow& row = table.CreateRow();
	cursor->GetRow(row);
	table.InsertRow(row);

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

}
TEST_SET_END
