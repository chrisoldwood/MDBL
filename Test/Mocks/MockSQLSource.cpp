////////////////////////////////////////////////////////////////////////////////
//! \file   MockSQLSource.cpp
//! \brief  The MockSQLSource class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "MockSQLSource.hpp"
#include "MockSQLCursor.hpp"

namespace Mocks
{

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

MockSQLSource::MockSQLSource()
	: m_isOpen(false)
	, m_inTransaction(false)
	, m_cursor(new MockSQLCursor)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

MockSQLSource::~MockSQLSource()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Set the cursor to return for any query.

void MockSQLSource::SetCursor(SQLCursorPtr cursor)
{
	m_cursor = cursor;
}

//
// CSQLSource interface.
//

void MockSQLSource::Open(const tchar* /*pszConnection*/)
{
	m_isOpen = true;
}

void MockSQLSource::Close()
{
	m_isOpen = false;
}

bool MockSQLSource::IsOpen() const
{
	return m_isOpen;
}

SQLParamsPtr MockSQLSource::CreateParams(const tchar* /*pszStmt*/, size_t /*nParams*/)
{
	CSQLParams* params = nullptr;

	return SQLParamsPtr(params);
}

void MockSQLSource::ExecStmt(const tchar* /*pszStmt*/)
{
}

void MockSQLSource::ExecStmt(const tchar* /*pszStmt*/, CSQLParams& /*oParams*/)
{
}

SQLCursorPtr MockSQLSource::ExecQuery(const tchar* /*pszQuery*/)
{
	return m_cursor;
}

bool MockSQLSource::InTrans()
{
	return m_inTransaction;
}

void MockSQLSource::BeginTrans()
{
	m_inTransaction = true;
}

void MockSQLSource::CommitTrans()
{
	m_inTransaction = false;
}

void MockSQLSource::RollbackTrans()
{
	m_inTransaction = false;
}

//namespace Mocks
}
