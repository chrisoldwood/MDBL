/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TABLE.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CTable class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef TABLE_HPP
#define TABLE_HPP

// Forward declarations.
class CMDB;

/******************************************************************************
** 
** The class used to hold a table in the database.
**
*******************************************************************************
*/

class CTable
{
public:
	//
	// Constructors/Destructor.
	//
	CTable(CMDB& oDB, const char* pszName, bool bTemp = false);
	virtual ~CTable();

	//
	// Accessors.
	//
	const CString& Name() const;
	bool Temporary() const;

	//
	// Column methods.
	//
	virtual int  ColumnCount() const;
	virtual const CColumn& Column(int n) const;
	virtual int AddColumn(const char* pszName, COLTYPE eType, int nLength, int nFlags = CColumn::DEFAULTS);
	virtual int AddColumn(const char* pszName, int nFlags, CTable& oTable, int nColumn);
	virtual void DropColumn(int nColumn);
	virtual void DropAllColumns();
	virtual int FindColumn(const char* pszName);

	//
	// Index methods.
	//
	virtual void AddIndex(int nColumn, CIndex::Type eType, int nApproxRows);
	virtual void DropIndex(int nColumn);

	//
	// Row methods.
	//
	virtual int   RowCount() const;
	virtual CRow& Row(int n) const;
	virtual CRow& operator[](int n) const;

	virtual CRow& CreateRow();
	virtual int   InsertRow(CRow& oRow);
	virtual void  DeleteRow(int nRow);
	virtual void  DeleteRow(CRow& oRow);
	virtual void  Truncate();

	//
	// Query methods.
	//
	virtual CResultSet Select(const CWhere& oWhere) const;
	virtual CRow*      SelectRow(int nColumn, const CValue& oValue) const;

	//
	// Persistance methods.
	//
	virtual bool Modified() const;

	virtual void operator <<(CStream& rStream);
	virtual void operator >>(CStream& rStream);

	virtual void operator <<(CSQLSource& rSource);
	virtual void operator >>(CSQLSource& rSource);

protected:
	//
	// Members.
	//
	CMDB&		m_oDB;			// The parent database.
	CString		m_strName;		// The name.
	bool		m_bTemp;		// Temporary table flag.
	CColumnSet	m_vColumns;		// The set of columns.
	CRowSet		m_vRows;		// The set of rows.
	bool		m_bInserted;	// Any rows inserted?
	bool		m_bDeleted;		// Any rows removed?
	int			m_nIdentCol;	// Identity column, if one.
	int32		m_nIdentVal;	// Next identity value.

	//
	// Friends.
	//
	friend class CRow;
	friend class CField;

	//
	// Template methods. (ala Triggers).
	//
	virtual void OnBeforeInsert(CRow& oRow);
	virtual void OnAfterInsert(CRow& oRow);
	virtual void OnBeforeDelete(CRow& oRow);
	virtual void OnAfterDelete(CRow& oRow);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline const CString& CTable::Name() const
{
	return m_strName;
}

inline bool CTable::Temporary() const
{
	return m_bTemp;
}

inline int CTable::ColumnCount() const
{
	return m_vColumns.Size();
}

inline const CColumn& CTable::Column(int n) const
{
	return m_vColumns.Column(n);
}

inline int CTable::FindColumn(const char* pszName)
{
	return m_vColumns.Find(pszName);
}

inline int CTable::RowCount() const
{
	return m_vRows.Size();
}

inline CRow& CTable::Row(int n) const
{
	return m_vRows.Row(n);
}

inline CRow& CTable::operator[](int n) const
{
	return m_vRows.Row(n);
}

#endif //TABLE_HPP
