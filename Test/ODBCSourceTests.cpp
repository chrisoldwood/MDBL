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

	CODBCSource source;
	source.Open(connection);

TEST_CASE("Executing a query returns a cursor to the result set")
{
	try
	{
		const tstring query = TXT("SELECT * FROM TestValues.csv");

		CSQLCursor* cursor = source.ExecQuery(query.c_str());

		TEST_TRUE(cursor != nullptr);

		delete cursor;
	}
	catch (const CODBCException& e)
	{
		TEST_FAILED_STR(e.m_strError);
	}
}
TEST_CASE_END

TEST_CASE("Executing a query throws when an error occurs")
{
	try
	{
		source.ExecQuery(TXT("a malformed SQL query"));

		TEST_FAILED("No exception thrown");
	}
	catch (const CODBCException&)
	{
		TEST_PASSED("Exception was thrown");
	}
}
TEST_CASE_END

}
TEST_SET_END
