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

#include <sql.h>
#include <sqlext.h>

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
	virtual void        ExecStmt(const char* pszStmt);
	virtual CSQLCursor* ExecQuery(const char* pszQuery);
	virtual void        ExecQuery(const char* pszQuery, CODBCCursor& oCursor);

	//
	// Data type conversion methods.
	//
	static COLTYPE     MDBType(SQLSMALLINT nODBCType);
	static SQLSMALLINT ODBCType(COLTYPE eMDBType);
	static int         BufferSize(const SQLColumn& oColumn);

protected:
	//
	// Members.
	//
	SQLHENV		m_hEnv;		// Environment handle.
	SQLHDBC		m_hDBC;		// Connection handle.

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
