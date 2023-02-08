////////////////////////////////////////////////////////////////////////////////
//! \file   SqlServerTests.cpp
//! \brief  The integration tests for the ODBC class accessing SQL Server.
//! \author Chris Oldwood
//! \note	These tests assume there is a local SQL Server Express database
//!			under the default instance name "SQLEXPRESS".

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <MDBL/ODBCSource.hpp>
#include <MDBL/ODBCCursor.hpp>
#include <MDBL/Table.hpp>
#include <MDBL/TimeStamp.hpp>

TEST_SET(SqlServer)
{
	tstring connection = TXT("Driver={SQL Server};"); // SQL Native Client
	connection += TXT("Server=.\\SQLEXPRESS;");
	connection += TXT("Trusted_Connection=Yes;");

	CODBCSource source(connection);

TEST_CASE("The SQL Server types are mapped to the MDBL column types")
{
	const tstring query =
	TXT("select")
    TXT("  cast(1 as bit)                               as [bit]")
    TXT(", cast(1 as tinyint)                           as [tinyint]")
    TXT(", cast(1 as smallint)                          as [smallint]")
    TXT(", cast(1 as int)                               as [int]")
    TXT(", cast(1 as bigint)                            as [bigint]")
    TXT(", cast(1.5 as decimal(2,1))                    as [decimal]")
	TXT(", cast('2006-05-04T01:02:03' as smalldatetime) as [smalldatetime]")
	TXT(", cast('2006-05-04T01:02:03' as datetime)      as [datetime]")
	TXT(", cast('fixed text' as char(10))               as [char]")
	TXT(", cast('variable text' as varchar(1024))       as [varchar]")
	TXT(";");

	CTable table(TXT("test"));

	table.AddColumn(TXT("bit"),           MDCT_BOOL,      0, CColumn::NULLABLE);
	table.AddColumn(TXT("tinyint"),       MDCT_INT,       0, CColumn::NULLABLE);
	table.AddColumn(TXT("smallint"),      MDCT_INT,       0, CColumn::NULLABLE);
	table.AddColumn(TXT("int"),           MDCT_INT,       0, CColumn::NULLABLE);
	table.AddColumn(TXT("bigint"),        MDCT_INT64,     0, CColumn::NULLABLE);
	table.AddColumn(TXT("decimal"),       MDCT_DOUBLE,    0, CColumn::NULLABLE);
	table.AddColumn(TXT("smalldatetime"), MDCT_TIMESTAMP, 0, CColumn::NULLABLE);
	table.AddColumn(TXT("datetime"),      MDCT_TIMESTAMP, 0, CColumn::NULLABLE);
	table.AddColumn(TXT("char"),          MDCT_FXDSTR,   10, CColumn::NULLABLE);
	table.AddColumn(TXT("varchar"),       MDCT_VARSTR, 1024, CColumn::NULLABLE);

	SQLCursorPtr cursor = source.ExecQuery(query);
	cursor->Fetch();

	CRow& row = table.CreateRow();
	cursor->GetRow(row);
	table.InsertRow(row);

	TEST_TRUE(row[0].GetBool() == true);
	TEST_TRUE(row[1].GetInt() == 1);
	TEST_TRUE(row[2].GetInt() == 1);
	TEST_TRUE(row[3].GetInt() == 1);
	TEST_TRUE(row[4].GetInt64() == 1);
	TEST_TRUE(row[5].GetDouble() == 1.5);
	TEST_TRUE(row[6].GetTimeStamp() == CTimeStamp(2006, 5, 4, 1, 2, 0));
	TEST_TRUE(row[7].GetTimeStamp() == CTimeStamp(2006, 5, 4, 1, 2, 3));
	TEST_TRUE(row[8].GetString() == tstring(TXT("fixed text")));
	TEST_TRUE(row[9].GetString() == tstring(TXT("variable text")));
}
TEST_CASE_END

}
TEST_SET_END
