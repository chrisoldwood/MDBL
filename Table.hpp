/******************************************************************************
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
	CTable(CMDB& oDB, const char* pszName, int nFlags = DEFAULTS);
	virtual ~CTable();

	//
	// Accessors.
	//
	const CString& Name() const;
	bool Transient() const;
	bool ReadOnly() const;

	//
	// Column methods.
	//
	virtual int  ColumnCount() const;
	virtual const CColumn& Column(int n) const;
	virtual int AddColumn(const char* pszName, COLTYPE eType, int nLength, int nFlags = CColumn::DEFAULTS);
	virtual int AddColumn(const char* pszName, CTable& oTable, int nColumn, int nFlags = CColumn::FOREIGNKEY);
	virtual void DropColumn(int nColumn);
	virtual void DropAllColumns();
	virtual int FindColumn(const char* pszName);

	//
	// Index methods.
	//
	virtual void AddIndex(int nColumn);
	virtual void DropIndex(int nColumn);

	//
	// Row methods.
	//
	virtual int   RowCount() const;
	virtual CRow& Row(int n) const;
	virtual CRow& operator[](int n) const;

	virtual CRow& CreateRow();
	virtual int   InsertRow(CRow& oRow, bool bNew = true);
	virtual void  DeleteRow(int nRow);
	virtual void  DeleteRow(CRow& oRow);
	virtual void  Truncate();

	virtual CRow& NullRow();
	virtual bool  IsNullRow(CRow& oRow) const;

	//
	// Query methods.
	//
	virtual CRow*      SelectRow(int nColumn, const CValue& oValue) const;
	virtual CResultSet Select(const CWhere& oQuery) const;
	virtual bool       Exists(const CWhere& oQuery) const;

	//
	// Persistance methods.
	//
	virtual bool Modified() const;

	virtual void operator <<(CStream& rStream);
	virtual void operator >>(CStream& rStream);

	virtual void operator <<(CSQLSource& rSource);
	virtual void operator >>(CSQLSource& rSource);

	//
	// Flags.
	//
	enum
	{
		PERSISTENT = 0x00,
		TRANSIENT  = 0x01,

		READ_WRITE = 0x00,
		READ_ONLY  = 0x02,

		DEFAULTS   = (PERSISTENT | READ_WRITE),
	};

protected:
	//
	// Members.
	//
	CMDB&		m_oDB;			// The parent database.
	CString		m_strName;		// The name.
	int			m_nFlags;		// Flags..
	CColumnSet	m_vColumns;		// The set of columns.
	CRowSet		m_vRows;		// The set of rows.
	int			m_nInsertions;	// Rows inserted.
	int			m_nDeletions;	// Rows removed.
	int			m_nIdentCol;	// Identity column, if one.
	int			m_nIdentVal;	// Next identity value.
	CRow*		m_pNullRow;		// The null row, if created.

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

	//
	// Internal methods.
	//
	virtual CString SQLQuery() const;
	virtual void    TruncateIndexes();

	//
	// Debug methods.
	//
	virtual void CheckIndexes() const;
	virtual void CheckRow(CRow& oRow) const;
	virtual void CheckColumn(CRow& oRow, int nColumn, const CValue& oValue) const;
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

inline bool CTable::Transient() const
{
	return (m_nFlags & TRANSIENT);
}

inline bool CTable::ReadOnly() const
{
	return (m_nFlags & READ_ONLY);
}

inline int CTable::ColumnCount() const
{
	return m_vColumns.Count();
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
	return m_vRows.Count();
}

inline CRow& CTable::Row(int n) const
{
	return m_vRows.Row(n);
}

inline CRow& CTable::operator[](int n) const
{
	return m_vRows.Row(n);
}

inline bool CTable::IsNullRow(CRow& oRow) const
{
	return (&oRow == m_pNullRow);
}

#endif //TABLE_HPP
