/******************************************************************************
**
** MODULE:		SQLSOURCE.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CSQLSource class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SQLSOURCE_HPP
#define SQLSOURCE_HPP

/******************************************************************************
** 
** This is the base class for all Database type Data Sources.
**
*******************************************************************************
*/

class CSQLSource
{
public:
	//
	// Constructors/Destructor.
	//
	CSQLSource();
	virtual ~CSQLSource();

	//
	// Connection methods.
	//
	virtual void Open(const char* pszConnection) = 0;
	virtual void Close() = 0;

	virtual bool IsOpen() const = 0;

	//
	// Statement methods.
	//
	virtual CSQLParams* CreateParams(const char* pszStmt, int nParams) = 0;
	virtual void        ExecStmt(const char* pszStmt) = 0;
	virtual void        ExecStmt(const char* pszStmt, CSQLParams& oParams) = 0;
	virtual CSQLCursor* ExecQuery(const char* pszQuery) = 0;

	//
	// Transaction methods.
	//
	virtual bool InTrans() = 0;
	virtual void BeginTrans() = 0;
	virtual void CommitTrans() = 0;
	virtual void RollbackTrans() = 0;

protected:
	//
	// Members.
	//
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CSQLSource::CSQLSource()
{
}

inline CSQLSource::~CSQLSource()
{
}

#endif //SQLSOURCE_HPP
