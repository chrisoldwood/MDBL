/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		COLUMN.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CColumn class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef COLUMN_HPP
#define COLUMN_HPP

// Forward declarations.
class CTable;
class CIndex;

/******************************************************************************
** 
** The base class used to store the attributes of a column in a table.
**
*******************************************************************************
*/

class CColumn
{
public:
	//
	// Accessors.
	//
	CTable& Table();
	const CString& Name() const;
	COLTYPE ColType() const;
	STGTYPE StgType() const;
	int     Length() const;
	int     AllocSize() const;
	int     Flags() const;
	bool    Nullable() const;
	bool    Unique() const;
	bool    ForeignKey() const;
	bool    PrimaryKey() const;
	bool    Persistant() const;
	bool    Writeable() const;
	CTable* FKTable() const;
	int     FKColumn() const;
	CIndex* Index() const;

	//
	// Mutators.
	//
	void Index(CIndex* pIndex);

	//
	// Flags.
	//
	enum
	{
		NOT_NULLABLE = 0x00,
		NULLABLE     = 0x01,

		NOT_UNIQUE   = 0x00,
		UNIQUE       = 0x02,

		NOT_KEY      = 0x00,
		FOREIGN_KEY  = 0x04,
		PRIMARY_KEY  = 0x08,

		PERSISTANT   = 0x00,
		TRANSIENT    = 0x10,

		READ_WRITE   = 0x00,
		READ_ONLY    = 0x20,

		DEFAULTS     = (NOT_NULLABLE | NOT_UNIQUE | NOT_KEY     | PERSISTANT | READ_WRITE),
		IDENTITY     = (NOT_NULLABLE | UNIQUE     | PRIMARY_KEY | PERSISTANT | READ_ONLY ),
		FOREIGNKEY   = (NOT_NULLABLE | NOT_UNIQUE | FOREIGN_KEY | PERSISTANT | READ_WRITE),
	};

	//
	// Class methods.
	//
	static STGTYPE ColToStgType(COLTYPE eType);

protected:
	//
	// Constructors/Destructor.
	//
	CColumn(CTable& oTable, const char* pszName, COLTYPE eType, int nLength, int nAllocSize, int nFlags = DEFAULTS);
	CColumn(CTable& oTable, const char* pszName, int nFlags, CTable& oFKTable, int nFKColumn, const CColumn& oFKColumn);
	virtual ~CColumn();

	//
	// Members.
	//
	CTable&	m_oTable;		// The parent table.
	CString	m_strName;		// Name.
	COLTYPE	m_eColType;		// The column type.
	STGTYPE	m_eStgType;		// The storage type.
	int		m_nLength;		// The data types length.
	int		m_nAllocSize;	// The data types allocation size.
	int		m_nFlags;		// Additional flags.
	CTable*	m_pFKTable;		// Foreign key table, if one.
	int		m_nFKColumn;	// Foreign key column, if one.
	CIndex*	m_pIndex;		// The index, if one.

	//
	// Friends.
	//
	friend class CColumnSet;
	friend class CTable;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CTable& CColumn::Table()
{
	return m_oTable;
}

inline const CString& CColumn::Name() const
{
	return m_strName;
}

inline COLTYPE CColumn::ColType() const
{
	return m_eColType;
}

inline STGTYPE CColumn::StgType() const
{
	return m_eStgType;
}

inline int CColumn::Length() const
{
	return m_nLength;
}

inline int CColumn::AllocSize() const
{
	return m_nAllocSize;
}

inline int CColumn::Flags() const
{
	return m_nFlags;
}

inline bool CColumn::Nullable() const
{
	return (m_nFlags & NULLABLE);
}

inline bool CColumn::Unique() const
{
	return (m_nFlags & UNIQUE);
}

inline bool CColumn::ForeignKey() const
{
	return (m_nFlags & FOREIGN_KEY);
}

inline bool	CColumn::PrimaryKey() const
{
	return (m_nFlags & PRIMARY_KEY);
}

inline bool	CColumn::Persistant() const
{
	return !(m_nFlags & TRANSIENT);
}

inline bool	CColumn::Writeable() const
{
	return !(m_nFlags & READ_ONLY);
}

inline CTable* CColumn::FKTable() const
{
	return m_pFKTable;
}

inline int CColumn::FKColumn() const
{
	return m_nFKColumn;
}

inline CIndex* CColumn::Index() const
{
	return m_pIndex;
}

#endif //COLUMN_HPP
