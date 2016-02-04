////////////////////////////////////////////////////////////////////////////////
//! \file   FieldTests.cpp
//! \brief  The unit tests for the Field class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <MDBL/MDB.hpp>
#include <MDBL/TimeStamp.hpp>

TEST_SET(Field)
{

TEST_CASE("A null field value will be formatted as an empty string")
{
	CMDB mdb;
	CTable table(mdb, TXT("test"));

	table.AddColumn(TXT("MDCT_INT"),       MDCT_INT,        0, CColumn::NULLABLE);
	table.AddColumn(TXT("MDCT_INT64"),     MDCT_INT64,      0, CColumn::NULLABLE);
	table.AddColumn(TXT("MDCT_DOUBLE"),    MDCT_DOUBLE,     0, CColumn::NULLABLE);
	table.AddColumn(TXT("MDCT_CHAR"),      MDCT_CHAR,       0, CColumn::NULLABLE);
	table.AddColumn(TXT("MDCT_FXDSTR"),    MDCT_FXDSTR,    42, CColumn::NULLABLE);
	table.AddColumn(TXT("MDCT_VARSTR"),    MDCT_VARSTR,    42, CColumn::NULLABLE);
	table.AddColumn(TXT("MDCT_BOOL"),      MDCT_BOOL,       0, CColumn::NULLABLE);
//	table.AddColumn(TXT("MDCT_IDENTITY"),  MDCT_IDENTITY,   0, CColumn::IDENTITY); // Cannot be null.
	table.AddColumn(TXT("MDCT_DATETIME"),  MDCT_DATETIME,   0, CColumn::NULLABLE);
	table.AddColumn(TXT("MDCT_DATE"),      MDCT_DATE,       0, CColumn::NULLABLE);
	table.AddColumn(TXT("MDCT_TIME"),      MDCT_TIME,       0, CColumn::NULLABLE);
	table.AddColumn(TXT("MDCT_TIMESTAMP"), MDCT_TIMESTAMP,  0, CColumn::NULLABLE);
	table.AddColumn(TXT("MDCT_VOIDPTR"),   MDCT_VOIDPTR,    0, CColumn::NULLABLE | CColumn::TRANSIENT);
	table.AddColumn(TXT("MDCT_ROWPTR"),    MDCT_ROWPTR,     0, CColumn::NULLABLE | CColumn::TRANSIENT);
	table.AddColumn(TXT("MDCT_ROWSETPTR"), MDCT_ROWSETPTR,  0, CColumn::NULLABLE | CColumn::TRANSIENT);

	CRow& row = table.CreateRow(true);
	table.InsertRow(row);

	for (size_t c = 0; c != table.ColumnCount(); ++c)
		TEST_TRUE(row[c].Format() == TXT(""));
}
TEST_CASE_END

TEST_CASE("A field value will be formatted using a default template when none provided")
{
	CMDB mdb;
	CTable table(mdb, TXT("test"));

	table.AddColumn(TXT("MDCT_INT"),       MDCT_INT,        0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_INT64"),     MDCT_INT64,      0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_DOUBLE"),    MDCT_DOUBLE,     0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_CHAR"),      MDCT_CHAR,       0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_FXDSTR"),    MDCT_FXDSTR,    42, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_VARSTR"),    MDCT_VARSTR,    42, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_BOOL"),      MDCT_BOOL,       0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_IDENTITY"),  MDCT_IDENTITY,   0, CColumn::IDENTITY);
	table.AddColumn(TXT("MDCT_DATETIME"),  MDCT_DATETIME,   0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_DATE"),      MDCT_DATE,       0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_TIME"),      MDCT_TIME,       0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_TIMESTAMP"), MDCT_TIMESTAMP,  0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_VOIDPTR"),   MDCT_VOIDPTR,    0, CColumn::DEFAULTS | CColumn::TRANSIENT);
	table.AddColumn(TXT("MDCT_ROWPTR"),    MDCT_ROWPTR,     0, CColumn::DEFAULTS | CColumn::TRANSIENT);
	table.AddColumn(TXT("MDCT_ROWSETPTR"), MDCT_ROWSETPTR,  0, CColumn::DEFAULTS | CColumn::TRANSIENT);

	CRow& row = table.CreateRow(false);
	table.InsertRow(row);

	const CTimeStamp timestamp(2006, 5, 4, 1, 2, 3);
	byte* pointer = 0;
	--pointer;

	row[ 0].SetInt(0);
	row[ 1].SetInt64(0);
	row[ 2].SetDouble(0);
	row[ 3].SetChar('0');
	row[ 4].SetString(TXT("0"));
	row[ 5].SetString(TXT("0"));
	row[ 6].SetBool(true);
//	row[ 7].SetInt(1); // Auto-generated
	row[ 8].SetDateTime(timestamp.ToTimeT());
	row[ 9].SetDateTime(timestamp.ToTimeT());
	row[10].SetDateTime(timestamp.ToTimeT());
	row[11].SetTimeStamp(timestamp);
	row[12].SetPtr(pointer);
	row[13].SetPtr(pointer);
	row[14].SetPtr(pointer);

	TEST_TRUE(row[ 0].Format() == TXT("0"));
	TEST_TRUE(row[ 1].Format() == TXT("0"));
	TEST_TRUE(row[ 2].Format() == TXT("0"));
	TEST_TRUE(row[ 3].Format() == TXT("0"));
	TEST_TRUE(row[ 4].Format() == TXT("0"));
	TEST_TRUE(row[ 5].Format() == TXT("0"));
	TEST_TRUE(row[ 6].Format() == TXT("Yes"));
	TEST_TRUE(row[ 7].Format() == TXT("1"));
	TEST_TRUE(row[ 8].Format() == TXT("04/05/06 01:02:03"));
	TEST_TRUE(row[ 9].Format() == TXT("04/05/06"));
	TEST_TRUE(row[10].Format() == TXT("01:02:03"));
	TEST_TRUE(row[11].Format() == TXT("04/05/06 01:02:03"));
	TEST_TRUE(row[12].Format() == TXT("FFFFFFFF"));
	TEST_TRUE(row[13].Format() == TXT("FFFFFFFF"));
	TEST_TRUE(row[14].Format() == TXT("FFFFFFFF"));
}
TEST_CASE_END

}
TEST_SET_END
