/******************************************************************************
**
** MODULE:		ODBCSOURCE.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CODBCSource class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef ODBCSOURCE_HPP
#define ODBCSOURCE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "MDBLTypes.hpp"
#include "SQLSource.hpp"
#include <sql.h>
#include <sqlext.h>

// Forward declarations.
class CODBCCursor;

/******************************************************************************
**
** This Data Source is used to access Databases via ODBC.
**
*******************************************************************************
*/

class CODBCSource : public CSQLSource
{
public:
	//
	// Constructors/Destructor.
	//
	CODBCSource();

	//! Construct the source and open the connection.
	CODBCSource(const tstring& connection); // throw(CODBCException)

	virtual ~CODBCSource();

	//
	// Connection methods.
	//
	virtual void Open(const tchar* pszConnection);
	using CSQLSource::Open; // throw(CODBCException)
	virtual void Close();

	virtual bool IsOpen() const;

	//
	// Statement methods.
	//
	virtual CSQLParams* CreateParams(const tchar* pszStmt, size_t nParams);
	virtual void        ExecStmt(const tchar* pszStmt);
	virtual void        ExecStmt(const tchar* pszStmt, CSQLParams& oParams);
	virtual SQLCursorPtr ExecQuery(const tchar* pszQuery);
	using CSQLSource::ExecQuery; // throw(CODBCException)
	virtual void        ExecQuery(const tchar* pszQuery, CODBCCursor& oCursor);

	//
	// Transaction methods.
	//
	virtual bool InTrans();
	virtual void BeginTrans();
	virtual void CommitTrans();
	virtual void RollbackTrans();

	//
	// Data type conversion methods.
	//
	static COLTYPE     MDBType(SQLSMALLINT nODBCType);
	static SQLSMALLINT ODBCType(COLTYPE eMDBType);
	static size_t      BufferSize(COLTYPE eColType, size_t nColSize);
	static size_t      ColumnSize(COLTYPE eColType, size_t nColSize);

	//
	// Installation query methods.
	//
	static void InstalledDrivers(CStrArray& astrDrivers);
	static void InstalledSources(CStrArray& astrSources);

protected:
	//
	// Members.
	//
	SQLHENV		m_hEnv;		// Environment handle.
	SQLHDBC		m_hDBC;		// Connection handle.
	bool		m_bInTrans;	// In a transaction?

	//
	// Friends.
	//
	friend class CODBCCursor;

private:
	// NotCopyable.
	CODBCSource(const CODBCSource&);
	CODBCSource& operator=(const CODBCSource&);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //ODBCSOURCE_HPP
