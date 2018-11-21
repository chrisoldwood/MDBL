////////////////////////////////////////////////////////////////////////////////
//! \file   MockSQLCursor.cpp
//! \brief  The MockSQLCursor class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "MockSQLCursor.hpp"

namespace Mocks
{

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

MockSQLCursor::MockSQLCursor()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

MockSQLCursor::~MockSQLCursor()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Set the number of columsn in the result set.
void MockSQLCursor::SetColumns(const Columns& columns)
{
	m_columns = columns;
}

size_t MockSQLCursor::NumColumns() const
{
	return m_columns.size();
}

//
// CSQLCursor interface.
//

const SQLColumn& MockSQLCursor::Column(size_t n) const
{
	return m_columns[n];
}

void MockSQLCursor::MapColumn(size_t /*sourceColumn*/, size_t /*destColumn*/, COLTYPE /*type*/, size_t /*size*/)
{
}

bool MockSQLCursor::Fetch()
{
	return false;
}

void MockSQLCursor::GetRow(CRow& /*oRow*/)
{
}

//namespace Mocks
}
