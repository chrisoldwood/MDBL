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

#if _MSC_VER > 1000
#pragma once
#endif

#include "ColumnSet.hpp"
#include "RowSet.hpp"

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
	CTable(CMDB& oDB, const tchar* pszName, uint nFlags = DEFAULTS);
	virtual ~CTable();

	//
	// Accessors.
	//
	const CString& Name() const;
	bool Transient() const;
	bool ReadOnly() const;

	CMDB& DB() const;

	//
	// Column methods.
	//
	virtual size_t ColumnCount() const;
	virtual const CColumn& Column(size_t n) const;
	virtual size_t AddColumn(const tchar* pszName, COLTYPE eType, size_t nLength, uint nFlags = CColumn::DEFAULTS);
	virtual size_t AddColumn(const tchar* pszName, CTable& oTable, size_t nColumn, uint nFlags = CColumn::FOREIGNKEY);
	virtual void DropColumn(size_t nColumn);
	virtual void DropAllColumns();
	virtual int FindColumn(const tchar* pszName);

	//
	// Index methods.
	//
	virtual void AddIndex(size_t nColumn);
	virtual void DropIndex(size_t nColumn);

	//
	// Row methods.
	//
	virtual size_t RowCount() const;
	virtual CRow& Row(size_t n) const;
	virtual CRow& operator[](size_t n) const;

	virtual CRow& CreateRow();
	virtual size_t InsertRow(CRow& oRow, bool bNew = true);
	virtual void  DeleteRow(size_t nRow);
	virtual void  DeleteRow(CRow& oRow);
	virtual void  DeleteRows(const CResultSet& oRS);
	virtual void  Truncate();
	virtual void  CopyTable(const CTable& oTable);

	virtual CRow& NullRow();
	virtual bool  IsNullRow(CRow& oRow) const;

	//
	// Query methods.
	//
	virtual CResultSet SelectAll() const;
	virtual CRow*      SelectRow(size_t nColumn, const CValue& oValue) const;
	virtual CResultSet Select(const CWhere& oQuery) const;
	virtual bool       Exists(const CWhere& oQuery) const;

	//
	// Save type flags.
	//
	enum RowTypes
	{
		INSERTED = 0x01,
		UPDATED  = 0x02,
		DELETED  = 0x04,

		ALL      = (INSERTED | UPDATED | DELETED),
	};

	//
	// Persistance methods.
	//
	virtual bool Modified() const;
	virtual void Modified(bool bModified);

	virtual void Read (WCL::IInputStream&  rStream);
	virtual void Write(WCL::IOutputStream& rStream);

	virtual void Read(CSQLSource& rSource);
	virtual void Write(CSQLSource& rSource, RowTypes eRows = ALL);

	virtual void ResetRowFlags();

	//
	// Table type flags.
	//
	enum
	{
		PERSISTENT = 0x00,
		TRANSIENT  = 0x01,

		READ_WRITE = 0x00,
		READ_ONLY  = 0x02,

		DEFAULTS   = (PERSISTENT | READ_WRITE),
	};

	//
	// Debug methods.
	//
	virtual void Dump(WCL::IOutputStream& rStream) const;

protected:
	//
	// Members.
	//
	CMDB&		m_oDB;			// The parent database.
	CString		m_strName;		// The name.
	uint		m_nFlags;		// Flags..
	CColumnSet	m_vColumns;		// The set of columns.
	CRowSet		m_vRows;		// The set of rows.
	size_t		m_nInsertions;	// Rows inserted.
	size_t		m_nUpdates;		// Fields updated.
	size_t		m_nDeletions;	// Rows removed.
	int			m_nIdentCol;	// Identity column, if one.
	int			m_nIdentVal;	// Next identity value.
	CRow*		m_pNullRow;		// The null row, if created.
	CString		m_strSQLTable;	// SQL table name, if different.
	CString		m_strSQLWhere;	// SQL WHERE clause.
	CString		m_strSQLGroup;	// SQL GROUP BY clause.
	CString		m_strSQLOrder;	// SQL ORDER BY clause.

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
	virtual CString SQLColumnList() const;
	virtual CString SQLQuery() const;
	virtual void    TruncateIndexes();
	virtual void    WriteInsertions(CSQLSource& rSource);
	virtual void    WriteUpdates(CSQLSource& rSource);
	virtual void    WriteDeletions(CSQLSource& rSource);

	//
	// Debug methods.
	//
	virtual void CheckIndexes() const;
	virtual void CheckRow(CRow& oRow, bool bUpdate) const;
	virtual void CheckColumn(CRow& oRow, size_t nColumn, const CValue& oValue, bool bUpdate) const;
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

inline CMDB& CTable::DB() const
{
	return m_oDB;
}

inline size_t CTable::ColumnCount() const
{
	return m_vColumns.Count();
}

inline const CColumn& CTable::Column(size_t n) const
{
	return m_vColumns.Column(n);
}

inline int CTable::FindColumn(const tchar* pszName)
{
	return m_vColumns.Find(pszName);
}

inline size_t CTable::RowCount() const
{
	return m_vRows.Count();
}

inline CRow& CTable::Row(size_t n) const
{
	return m_vRows.Row(n);
}

inline CRow& CTable::operator[](size_t n) const
{
	return m_vRows.Row(n);
}

inline bool CTable::IsNullRow(CRow& oRow) const
{
	return (&oRow == m_pNullRow);
}

#endif //TABLE_HPP
