/******************************************************************************
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

#if _MSC_VER > 1000
#pragma once
#endif

#include "FwdDecls.hpp"
#include "MDBLTypes.hpp"

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
	CTable& Table() const;
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
	bool    ReadOnly() const;
	bool	Transient() const;
	CTable* FKTable() const;
	int     FKColumn() const;
	CIndex* Index() const;
	int     DisplayWidth(bool bDebug = false) const;

	//
	// Mutators.
	//
	void Index(CIndex* pIndex);

	//
	// Flags.
	//
	enum
	{
		// Standard flags.
		NOT_NULLABLE = 0x0000,
		NULLABLE     = 0x0001,

		NOT_UNIQUE   = 0x0000,
		UNIQUE       = 0x0002,

		NOT_KEY      = 0x0000,
		FOREIGN_KEY  = 0x0004,
		PRIMARY_KEY  = 0x0008,

		READ_WRITE   = 0x0000,
		READ_ONLY    = 0x0010,

		PERSISTENT   = 0x0000,
		TRANSIENT    = 0x0020,

		// Type specific flags.
		IGNORE_CASE  = 0x0000,
		COMPARE_CASE = 0x0100,

		TZ_GMT       = 0x0000,
		TZ_LOCAL     = 0x0100,

		DEFAULTS     = (NOT_NULLABLE | NOT_UNIQUE | NOT_KEY     | READ_WRITE | PERSISTENT),
		IDENTITY     = (NOT_NULLABLE | UNIQUE     | PRIMARY_KEY | READ_ONLY  | PERSISTENT),
		FOREIGNKEY   = (NOT_NULLABLE | NOT_UNIQUE | FOREIGN_KEY | READ_WRITE | PERSISTENT),
	};

	//
	// Class methods.
	//
	static STGTYPE ColToStgType(COLTYPE eType);

protected:
	//
	// Constructors/Destructor.
	//
	CColumn(CTable& oTable, const char* pszName, COLTYPE eType, int nLength, int nAllocSize, int nFlags);
	CColumn(CTable& oTable, const char* pszName, CTable& oFKTable, int nFKColumn, const CColumn& oFKColumn, int nFlags);
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

inline CTable& CColumn::Table() const
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

inline bool	CColumn::ReadOnly() const
{
	return (m_nFlags & READ_ONLY);
}

inline bool CColumn::Transient() const
{
	return (m_nFlags & TRANSIENT);
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
