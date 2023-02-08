////////////////////////////////////////////////////////////////////////////////
//! \file   ODBCSourceTests.cpp
//! \brief  The integration tests for the CODBCSource class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <MDBL/ODBCSource.hpp>
#include <MDBL/ODBCCursor.hpp>
#include <WCL/Path.hpp>

TEST_SET(ODBCSource)
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

TEST_CASE("A source is not open by default")
{
	CODBCSource source;

	TEST_FALSE(source.IsOpen());
}
TEST_CASE_END

TEST_CASE("A source can be opened manually")
{
	CODBCSource source;

	source.Open(connection);

	TEST_TRUE(source.IsOpen());
}
TEST_CASE_END

TEST_CASE("A source can be opened on construction")
{
	CODBCSource source(connection);

	TEST_TRUE(source.IsOpen());
}
TEST_CASE_END

TEST_CASE("Failing to open a source throws an exception")
{
	CODBCSource source;

	TEST_THROWS(source.Open(TXT("malformed connection string")));
}
TEST_CASE_END

	CODBCSource source(connection);

TEST_CASE("Executing a query returns a cursor to the result set")
{
	const tstring query = TXT("SELECT * FROM TestValues.csv");

	SQLCursorPtr cursor = source.ExecQuery(query);

	TEST_FALSE(cursor.empty());
}
TEST_CASE_END

TEST_CASE("Executing a query throws when an error occurs")
{
	TEST_THROWS(source.ExecQuery(TXT("a malformed SQL query")));
}
TEST_CASE_END

}
TEST_SET_END
