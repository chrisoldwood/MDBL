////////////////////////////////////////////////////////////////////////////////
//! \file   MockSQLSource.hpp
//! \brief  The MockSQLSource class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_MOCKSQLSOURCE_HPP
#define APP_MOCKSQLSOURCE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <MDBL/SQLSource.hpp>

namespace Mocks
{

////////////////////////////////////////////////////////////////////////////////
//! A mock SQL source.

class MockSQLSource : public CSQLSource
{
public:
	//! Default constructor.
	MockSQLSource();

	//! Destructor.
	~MockSQLSource();

	//
	// Properties.
	//

	//! Set the cursor to return for any query.
	void SetCursor(SQLCursorPtr cursor);

	//
	// CSQLSource interface.
	//

	virtual void Open(const tchar* pszConnection);
	virtual void Close();
	virtual bool IsOpen() const;

	virtual SQLParamsPtr CreateParams(const tchar* pszStmt, size_t nParams);
	virtual void        ExecStmt(const tchar* pszStmt);
	virtual void        ExecStmt(const tchar* pszStmt, CSQLParams& oParams);
	virtual SQLCursorPtr ExecQuery(const tchar* pszQuery);

	virtual bool InTrans();
	virtual void BeginTrans();
	virtual void CommitTrans();
	virtual void RollbackTrans();

private:
	//
	// Members.
	//
	bool			m_isOpen;			//!< Is the connection open?
	bool			m_inTransaction;	//!< Are we inside a transaction?
	SQLCursorPtr	m_cursor;			//!< The cursor to return for any query.
};

//namespace Mocks
}

#endif // APP_MOCKSQLSOURCE_HPP
