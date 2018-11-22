////////////////////////////////////////////////////////////////////////////////
//! \file   MockSQLCursor.hpp
//! \brief  The MockSQLCursor class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_MOCKSQLCURSOR_HPP
#define APP_MOCKSQLCURSOR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <MDBL/SQLCursor.hpp>
#include <vector>

namespace Mocks
{

////////////////////////////////////////////////////////////////////////////////
//! A mock SQL result set column.

struct MockSQLColumn : public SQLColumn
{
	MockSQLColumn(size_t index, CString name, COLTYPE type, size_t size, uint flags)
	 : SQLColumn(index, name, type, size, flags)
	{}
};

////////////////////////////////////////////////////////////////////////////////
//! A mock SQL result set cursor.

class MockSQLCursor : public CSQLCursor
{
public:
	//
	// Types.
	//

	//! The collection of columns in the result set.
	typedef std::vector<MockSQLColumn> Columns;

public:
	//! Default constructor.
	MockSQLCursor();

	//! Destructor.
	~MockSQLCursor();
	
	//
	// Properties.
	//

	//! Set the number of columsn in the result set.
	void SetColumns(const Columns& columns);

	//
	// CSQLCursor interface.
	//
	virtual size_t NumColumns() const;
	virtual const SQLColumn& Column(size_t n) const;
	virtual void MapColumn(size_t sourceColumn, size_t destColumn, COLTYPE type, size_t size);
	virtual bool Fetch();
	virtual void GetRow(CRow& oRow);

private:
	//
	// Members.
	//
	Columns	m_columns;	//!< The collection of columns in the result set.
};

//! The default MockSQLCursor smart pointer type.
typedef Core::SharedPtr<MockSQLCursor> MockSQLCursorPtr;

//namespace Mocks
}

#endif // APP_MOCKSQLCURSOR_HPP
