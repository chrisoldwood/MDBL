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
	virtual ~CODBCSource();
	
	//
	// Connection methods.
	//
	virtual void Open(const char* pszConnection);
	virtual void Close();

	virtual bool IsOpen() const;

	//
	// Statement methods.
	//
	virtual CSQLParams* CreateParams(const char* pszStmt, int nParams);
	virtual void        ExecStmt(const char* pszStmt);
	virtual void        ExecStmt(const char* pszStmt, CSQLParams& oParams);
	virtual CSQLCursor* ExecQuery(const char* pszQuery);
	virtual void        ExecQuery(const char* pszQuery, CODBCCursor& oCursor);

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
	static int         BufferSize(COLTYPE eColType, int nColSize);
	static int         ColumnSize(COLTYPE eColType, int nColSize);

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
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //ODBCSOURCE_HPP
