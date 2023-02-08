////////////////////////////////////////////////////////////////////////////////
//! \file   FieldTests.cpp
//! \brief  The unit tests for the Field class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <MDBL/Table.hpp>
#include <MDBL/TimeStamp.hpp>

TEST_SET(Field)
{
	const CTimeStamp timestamp(2006, 5, 4, 1, 2, 3);
	const time_t timet = timestamp.ToTimeT();
	byte* pointer = 0;
	--pointer;
	CRow* rowPtr = reinterpret_cast<CRow*>(pointer);
	CRow** rowSet = reinterpret_cast<CRow**>(pointer);

TEST_CASE("A nullable column will contain a null value when the row is first created and null-by-default is requested")
{
	CTable table(TXT("test"));

	table.AddColumn(TXT("MDCT_INT"),       MDCT_INT,        0, CColumn::NULLABLE);
	table.AddColumn(TXT("MDCT_INT64"),     MDCT_INT64,      0, CColumn::NULLABLE);
	table.AddColumn(TXT("MDCT_DOUBLE"),    MDCT_DOUBLE,     0, CColumn::NULLABLE);
	table.AddColumn(TXT("MDCT_CHAR"),      MDCT_CHAR,       0, CColumn::NULLABLE);
	table.AddColumn(TXT("MDCT_FXDSTR"),    MDCT_FXDSTR,    42, CColumn::NULLABLE);
	table.AddColumn(TXT("MDCT_VARSTR"),    MDCT_VARSTR,    42, CColumn::NULLABLE);
	table.AddColumn(TXT("MDCT_BOOL"),      MDCT_BOOL,       0, CColumn::NULLABLE);
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
		TEST_TRUE(row[c] == null);
}
TEST_CASE_END

TEST_CASE("A null field value will be formatted as an empty string")
{
	CTable table(TXT("test"));

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

TEST_CASE("A field can be mutated and queried")
{
	CTable table(TXT("test"));

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
	table.AddColumn(TXT("NULLABLE"),       MDCT_INT,        0, CColumn::NULLABLE);

	CRow& row = table.CreateRow(false);
	table.InsertRow(row);

	row[ 0].SetInt(1);
	row[ 1].SetInt64(1);
	row[ 2].SetDouble(1.0);
	row[ 3].SetChar('X');
	row[ 4].SetString(TXT("X"));
	row[ 5].SetString(TXT("X"));
	row[ 6].SetBool(true);
//	row[ 7].SetInt(1); // Auto-generated
	row[ 8].SetTimeT(timet);
	row[ 9].SetTimeT(timet);
	row[10].SetTimeT(timet);
	row[11].SetTimeStamp(timestamp);
	row[12].SetPtr(pointer);
	row[13].SetRowPtr(rowPtr);
	row[14].SetRowSetPtr(rowSet);
	row[15].SetNull();

	TEST_TRUE(row[ 0].GetInt() == 1);
	TEST_TRUE(row[ 1].GetInt64() == 1);
	TEST_TRUE(row[ 2].GetDouble() == 1.0);
	TEST_TRUE(row[ 3].GetChar() == TXT('X'));
	TEST_TRUE(tstrcmp(row[ 4].GetString(), TXT("X")) == 0);
	TEST_TRUE(tstrcmp(row[ 5].GetString(), TXT("X")) == 0);
	TEST_TRUE(row[ 6].GetBool() == true);
	TEST_TRUE(row[ 7].GetInt() == 1);
	TEST_TRUE(row[ 8].GetInt64() == timet);
	TEST_TRUE(row[ 9].GetInt64() == timet);
	TEST_TRUE(row[10].GetInt64() == timet);
	TEST_TRUE(row[11].GetTimeStamp() == timestamp);
	TEST_TRUE(row[12].GetPtr() == pointer);
	TEST_TRUE(row[13].GetRowPtr() == rowPtr);
	TEST_TRUE(row[14].GetRowSetPtr() == rowSet);
	TEST_TRUE(row[15].IsNull() == true);
}
TEST_CASE_END

TEST_CASE("A field value will be formatted using a default template when none provided")
{
	CTable table(TXT("test"));

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

	row[ 0].SetInt(0);
	row[ 1].SetInt64(0);
	row[ 2].SetDouble(0);
	row[ 3].SetChar('0');
	row[ 4].SetString(TXT("0"));
	row[ 5].SetString(TXT("0"));
	row[ 6].SetBool(true);
//	row[ 7].SetInt(1); // Auto-generated
	row[ 8].SetTimeT(timestamp.ToTimeT());
	row[ 9].SetTimeT(timestamp.ToTimeT());
	row[10].SetTimeT(timestamp.ToTimeT());
	row[11].SetTimeStamp(timestamp);
	row[12].SetPtr(pointer);
	row[13].SetPtr(rowPtr);
	row[14].SetPtr(rowSet);

	TEST_TRUE(row[ 0].Format() == TXT("0"));
	TEST_TRUE(row[ 1].Format() == TXT("0"));
#ifdef _MSC_VER
	TEST_TRUE(row[ 2].Format() == TXT("0"));
#else
//	TEST_TRUE(row[ 2].Format() == TXT("0")); // MinGW GCC 5.1.0 currently returns "g", why?
#endif
	TEST_TRUE(row[ 3].Format() == TXT("0"));
	TEST_TRUE(row[ 4].Format() == TXT("0"));
	TEST_TRUE(row[ 5].Format() == TXT("0"));
	TEST_TRUE(row[ 6].Format() == TXT("Yes"));
	TEST_TRUE(row[ 7].Format() == TXT("1"));
	TEST_TRUE(row[ 8].Format() == TXT("04/05/06 01:02:03"));
	TEST_TRUE(row[ 9].Format() == TXT("04/05/06"));
	TEST_TRUE(row[10].Format() == TXT("01:02:03"));
	TEST_TRUE(row[11].Format() == TXT("04/05/06 01:02:03"));
#ifndef _WIN64 
	TEST_TRUE(row[12].Format() == TXT("FFFFFFFF"));
	TEST_TRUE(row[13].Format() == TXT("FFFFFFFF"));
	TEST_TRUE(row[14].Format() == TXT("FFFFFFFF"));
#else
	TEST_TRUE(row[12].Format() == TXT("FFFFFFFFFFFFFFFF"));
	TEST_TRUE(row[13].Format() == TXT("FFFFFFFFFFFFFFFF"));
	TEST_TRUE(row[14].Format() == TXT("FFFFFFFFFFFFFFFF"));
#endif
}
TEST_CASE_END

TEST_CASE("A field can be converted to a value")
{
	CTable table(TXT("test"));

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

	row[ 0].SetInt(1);
	row[ 1].SetInt64(1);
	row[ 2].SetDouble(1.0);
	row[ 3].SetChar('X');
	row[ 4].SetString(TXT("X"));
	row[ 5].SetString(TXT("X"));
	row[ 6].SetBool(true);
//	row[ 7].SetInt(1); // Auto-generated
	row[ 8].SetTimeT(timet);
	row[ 9].SetTimeT(timet);
	row[10].SetTimeT(timet);
	row[11].SetTimeStamp(timestamp);
	row[12].SetPtr(pointer);
	row[13].SetRowPtr(rowPtr);
	row[14].SetRowSetPtr(rowSet);

	TEST_TRUE(row[ 0].ToValue().m_iValue == 1);
	TEST_TRUE(row[ 1].ToValue().m_i64Value == 1);
	TEST_TRUE(row[ 2].ToValue().m_dValue == 1.0);
	TEST_TRUE(row[ 3].ToValue().m_cValue == TXT('X'));
	TEST_TRUE(tstrcmp(row[ 4].ToValue().m_sValue, TXT("X")) == 0);
	TEST_TRUE(tstrcmp(row[ 5].ToValue().m_sValue, TXT("X")) == 0);
	TEST_TRUE(row[ 6].ToValue().m_bValue == true);
	TEST_TRUE(row[ 7].ToValue().m_iValue == 1);
	TEST_TRUE(row[ 8].ToValue().m_i64Value == timet);
	TEST_TRUE(row[ 9].ToValue().m_i64Value == timet);
	TEST_TRUE(row[10].ToValue().m_i64Value == timet);
	TEST_TRUE(row[11].ToValue().m_i64Value == timet);
	TEST_TRUE(row[12].ToValue().m_pValue == pointer);
	TEST_TRUE(row[13].ToValue().m_pValue == rowPtr);
	TEST_TRUE(row[14].ToValue().m_pValue == rowSet);
}
TEST_CASE_END

TEST_CASE("A field can be implicitly assigned to")
{
	CTable table(TXT("test"));

	table.AddColumn(TXT("MDCT_INT"),       MDCT_INT,        0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_INT64"),     MDCT_INT64,      0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_DOUBLE"),    MDCT_DOUBLE,     0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_CHAR"),      MDCT_CHAR,       0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_FXDSTR"),    MDCT_FXDSTR,    42, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_VARSTR"),    MDCT_VARSTR,    42, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_BOOL"),      MDCT_BOOL,       0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_DATETIME"),  MDCT_DATETIME,   0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_DATE"),      MDCT_DATE,       0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_TIME"),      MDCT_TIME,       0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_TIMESTAMP"), MDCT_TIMESTAMP,  0, CColumn::DEFAULTS);
	table.AddColumn(TXT("MDCT_VOIDPTR"),   MDCT_VOIDPTR,    0, CColumn::DEFAULTS | CColumn::TRANSIENT);
	table.AddColumn(TXT("MDCT_ROWPTR"),    MDCT_ROWPTR,     0, CColumn::DEFAULTS | CColumn::TRANSIENT);
	table.AddColumn(TXT("MDCT_ROWSETPTR"), MDCT_ROWSETPTR,  0, CColumn::DEFAULTS | CColumn::TRANSIENT);
	table.AddColumn(TXT("NULLABLE"),       MDCT_INT,        0, CColumn::NULLABLE);

	CRow& row = table.CreateRow(false);
	table.InsertRow(row);

	row[ 0] = 1;
	row[ 1] = 1i64;
	row[ 2] = 1.0;
	row[ 3] = TXT('X');
	row[ 4] = TXT("X");
	row[ 5] = TXT("X");
	row[ 6] = true;
	row[ 7] = timet;
	row[ 8] = timet;
	row[ 9] = timet;
	row[10] = timestamp;
	row[11] = pointer;
	row[12] = rowPtr;
	row[13] = rowSet;
	row[14] = null;

	TEST_TRUE(row[ 0].GetInt() == 1);
	TEST_TRUE(row[ 1].GetInt64() == 1);
	TEST_TRUE(row[ 2].GetDouble() == 1.0);
	TEST_TRUE(row[ 3].GetChar() == TXT('X'));
	TEST_TRUE(tstrcmp(row[ 4].GetString(), TXT("X")) == 0);
	TEST_TRUE(tstrcmp(row[ 5].GetString(), TXT("X")) == 0);
	TEST_TRUE(row[ 6].GetBool() == true);
	TEST_TRUE(row[ 7].GetInt64() == timet);
	TEST_TRUE(row[ 8].GetInt64() == timet);
	TEST_TRUE(row[ 9].GetInt64() == timet);
	TEST_TRUE(row[10].GetTimeStamp() == timestamp);
	TEST_TRUE(row[11].GetPtr() == pointer);
	TEST_TRUE(row[12].GetRowPtr() == rowPtr);
	TEST_TRUE(row[13].GetRowSetPtr() == rowSet);
	TEST_TRUE(row[14].IsNull() == true);
}
TEST_CASE_END

TEST_CASE("A field can be implictly converted to a value of the column's storage type")
{
	CTable table(TXT("test"));

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

	row[ 0].SetInt(1);
	row[ 1].SetInt64(1);
	row[ 2].SetDouble(1.0);
	row[ 3].SetChar('X');
	row[ 4].SetString(TXT("X"));
	row[ 5].SetString(TXT("X"));
	row[ 6].SetBool(true);
//	row[ 7].SetInt(1); // Auto-generated
	row[ 8].SetTimeT(timet);
	row[ 9].SetTimeT(timet);
	row[10].SetTimeT(timet);
	row[11].SetTimeStamp(timestamp);
	row[12].SetPtr(pointer);
	row[13].SetRowPtr(rowPtr);
	row[14].SetRowSetPtr(rowSet);

	TEST_TRUE(static_cast<int>(row[ 0]) == 1);
	TEST_TRUE(static_cast<int64>(row[ 1]) == 1);
	TEST_TRUE(static_cast<double>(row[ 2]) == 1.0);
	TEST_TRUE(static_cast<tchar>(row[ 3]) == TXT('X'));
	TEST_TRUE(tstrcmp(static_cast<const tchar*>(row[ 4]), TXT("X")) == 0);
	TEST_TRUE(tstrcmp(static_cast<const tchar*>(row[ 5]), TXT("X")) == 0);
	TEST_TRUE(static_cast<bool>(row[ 6]) == true);
	TEST_TRUE(static_cast<int>(row[ 7]) == 1);
	TEST_TRUE(static_cast<int64>(row[ 8]) == timet);
	TEST_TRUE(static_cast<int64>(row[ 9]) == timet);
	TEST_TRUE(static_cast<int64>(row[10]) == timet);
	TEST_TRUE(static_cast<CTimeStamp>(row[11]) == timestamp);
	TEST_TRUE(static_cast<void*>(row[12]) == pointer);
	TEST_TRUE(static_cast<CRow*>(row[13]) == rowPtr);
	TEST_TRUE(static_cast<CRow**>(row[14]) == rowSet);
}
TEST_CASE_END

}
TEST_SET_END
