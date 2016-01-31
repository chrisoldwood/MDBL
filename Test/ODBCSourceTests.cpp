////////////////////////////////////////////////////////////////////////////////
//! \file   ODBCSourceTests.cpp
//! \brief  The integration tests for the ODBC driver component.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <MDBL/ODBCSource.hpp>
#include <MDBL/ODBCCursor.hpp>
#include <WCL/Path.hpp>
#include <MDBL/ODBCException.hpp>

TEST_SET(ODBCSource)
{
	const CPath testRunnerFolder = CPath::ApplicationDir();
	const CPath databaseFolder = testRunnerFolder.Parent() / TXT("Database");

	ASSERT(databaseFolder.Exists());

	tstring connection = TXT("Driver={Microsoft Text Driver (*.txt; *.csv)};");
	connection += Core::fmt(TXT("Dbq=%s;"), databaseFolder.c_str());

	CODBCSource source(connection);

TEST_CASE("Executing a query returns a cursor to the result set")
{
	const tstring query = TXT("SELECT * FROM TestValues.csv");

	SQLCursorPtr cursor = source.ExecQuery(query.c_str());

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
