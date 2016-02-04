////////////////////////////////////////////////////////////////////////////////
//! \file   TimeStampTests.cpp
//! \brief  The unit tests for the TimeStamp class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <MDBL/TimeStamp.hpp>

TEST_SET(TimeStamp)
{

TEST_CASE("A timestamp has all fields set to zero by default")
{
	const CTimeStamp timestamp;

	TEST_TRUE(timestamp.year     == 0);
	TEST_TRUE(timestamp.month    == 0);
	TEST_TRUE(timestamp.day      == 0);
	TEST_TRUE(timestamp.hour     == 0);
	TEST_TRUE(timestamp.minute   == 0);
	TEST_TRUE(timestamp.second   == 0);
	TEST_TRUE(timestamp.fraction == 0);
}
TEST_CASE_END

TEST_CASE("A timestamp can be constructed from the discrete set of fields")
{
	const CTimeStamp timestamp(2001, 2, 3, 4, 5, 6, 7);

	TEST_TRUE(timestamp.year     == 2001);
	TEST_TRUE(timestamp.month    == 2);
	TEST_TRUE(timestamp.day      == 3);
	TEST_TRUE(timestamp.hour     == 4);
	TEST_TRUE(timestamp.minute   == 5);
	TEST_TRUE(timestamp.second   == 6);
	TEST_TRUE(timestamp.fraction == 7);
}
TEST_CASE_END

TEST_CASE("two timestamps can be compared for equality")
{
	const CTimeStamp lhs(2001, 2, 3, 4, 5, 6, 7);
	const CTimeStamp rhs(2001, 2, 3, 4, 5, 6, 7);

	TEST_TRUE(lhs == rhs);
}
TEST_CASE_END

TEST_CASE("two timestamps can be compared for inequality")
{
	const CTimeStamp lhs;
	const CTimeStamp rhs(2001, 2, 3, 4, 5, 6, 7);

	TEST_TRUE(lhs != rhs);
}
TEST_CASE_END

TEST_CASE("a timestamp can be converted to and from a local time_t value")
{
	const CTimeStamp original(2001, 12, 31, 4, 5, 6, 0);

	time_t time = original.ToTimeT();
	
	CTimeStamp copy;

	copy.FromTimeT(time);

	TEST_TRUE(copy == original);
}
TEST_CASE_END

TEST_CASE("a timestamp can be formatted as a string")
{
	const CTimeStamp timestamp(2001, 12, 31, 4, 5, 6, 500000000);

	TEST_TRUE(timestamp.ToString() == TXT("2001-12-31 04:05:06.500"));
}
TEST_CASE_END

}
TEST_SET_END
