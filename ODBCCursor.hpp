/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		ODBCCURSOR.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CODBCCursor class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef ODBCCURSOR_HPP
#define ODBCCURSOR_HPP

#include <sql.h>
#include <sqlext.h>

// Forward declarations.
class CODBCSource;

/******************************************************************************
** 
** This is the SQL cursor type returned for ODBC queries.
**
*******************************************************************************
*/

class CODBCCursor : public CSQLCursor
{
public:
	//
	// Constructors/Destructor.
	//
	CODBCCursor(const CODBCSource& oSource);
	virtual ~CODBCCursor();

	virtual void Open(SQLHSTMT hStmt);
	virtual void Close();
	
	//
	// Accessors.
	//
	virtual bool IsOpen() const;
	virtual int NumColumns() const;
	virtual SQLColumn& Column(int n) const;

	virtual bool Fetch();
	virtual void SetRow(CRow& oRow);

protected:
	// Row fetch size.
	enum { FETCH_SIZE = 10 };

	//
	// Members.
	//
	const CODBCSource*	m_pSource;		// The data source.
	SQLHSTMT			m_hStmt;		// The statement handle.
	SQLSMALLINT			m_nColumns;		// Number of columns returned.
	SQLColumn*			m_pColumns;		// The array of column definitions.
	int					m_nRowLen;		// Size of a row.
	int					m_nTotalLen;	// Total size of the buffer.
	int*				m_pOffsets;		// The array of value offsets.
	byte*				m_pRowData;		// The row data.
	SQLUSMALLINT*		m_pRowStatus;	// The array of status values.
	bool				m_bDoneBind;	// Bind output buffers flag.
	SQLUINTEGER			m_nFetched;		// Number of rows fetched.
	SQLUINTEGER			m_nCurRow;		// Current row 

	//
	// Internal methods.
	//
	virtual void Bind();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //ODBCCURSOR_HPP
