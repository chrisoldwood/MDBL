////////////////////////////////////////////////////////////////////////////////
//! \file   ValueTests.cpp
//! \brief  The unit tests for the Value class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <MDBL/Value.hpp>

TEST_SET(Value)
{

TEST_CASE("Value created with special 'null' value has type NULL")
{
	const CValue value(null);

	TEST_TRUE(value.m_eType == MDST_NULL);
	TEST_TRUE(value.m_bNull);
}
TEST_CASE_END

TEST_CASE("Value can be created with a boolean value")
{
	const bool rawValue = true;

	const CValue value(rawValue);

	TEST_TRUE(value.m_eType == MDST_BOOL);
	TEST_FALSE(value.m_bNull);
	TEST_TRUE(value.m_bValue == rawValue);
}
TEST_CASE_END

TEST_CASE("Value can be created from all signed integer value types")
{
{
	int rawValue = 42;

	const CValue value(rawValue);

	TEST_TRUE(value.m_eType == MDST_INT);
	TEST_FALSE(value.m_bNull);
	TEST_TRUE(value.m_iValue == rawValue);
}
{
	short rawValue = 42;

	const CValue value(rawValue);

	TEST_TRUE(value.m_eType == MDST_INT);
	TEST_FALSE(value.m_bNull);
	TEST_TRUE(value.m_iValue == rawValue);
}
{
	long rawValue = 42;

	const CValue value(rawValue);

	TEST_TRUE(value.m_eType == MDST_INT);
	TEST_FALSE(value.m_bNull);
	TEST_TRUE(value.m_iValue == rawValue);
}
{
	int64 rawValue = 42;

	const CValue value(rawValue);

	TEST_TRUE(value.m_eType == MDST_INT64);
	TEST_FALSE(value.m_bNull);
	TEST_TRUE(value.m_i64Value == rawValue);
}
}
TEST_CASE_END

TEST_CASE("Value can be created from all floting point types")
{
{
	float rawValue = 42;

	const CValue value(rawValue);

	TEST_TRUE(value.m_eType == MDST_DOUBLE);
	TEST_FALSE(value.m_bNull);
	TEST_TRUE(value.m_dValue == rawValue);
}
{
	double rawValue = 42;

	const CValue value(rawValue);

	TEST_TRUE(value.m_eType == MDST_DOUBLE);
	TEST_FALSE(value.m_bNull);
	TEST_TRUE(value.m_dValue == rawValue);
}
}
TEST_CASE_END

TEST_CASE("Value can be created with a single character value")
{
	const tchar rawValue = 'v';

	const CValue value(rawValue);

	TEST_TRUE(value.m_eType == MDST_CHAR);
	TEST_FALSE(value.m_bNull);
	TEST_TRUE(value.m_cValue == rawValue);
}
TEST_CASE_END

TEST_CASE("Value can be created from a string value")
{
{
	const tchar* rawValue = TXT("value");

	const CValue value(rawValue);

	TEST_TRUE(value.m_eType == MDST_STRING);
	TEST_FALSE(value.m_bNull);
	TEST_TRUE(tstrcmp(value.m_sValue, rawValue) == 0);
}
{
	const CString rawValue = TXT("value");

	const CValue value(rawValue);

	TEST_TRUE(value.m_eType == MDST_STRING);
	TEST_FALSE(value.m_bNull);
	TEST_TRUE(tstrcmp(value.m_sValue, rawValue) == 0);
}
}
TEST_CASE_END

}
TEST_SET_END
