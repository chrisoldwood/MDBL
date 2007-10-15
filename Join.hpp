/******************************************************************************
**
** MODULE:		JOIN.H
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CJoin class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef JOIN_HPP
#define JOIN_HPP

#if _MSC_VER > 1000
#pragma once
#endif

/******************************************************************************
** 
** The class used to hold a table used in a CJoin query.
**
*******************************************************************************
*/

class CJoinTable
{
public:
	//
	// Constructors/Destructor.
	//
	CJoinTable(int nTable, int nLHSColumn, int nType, int nRHSColumn);
	~CJoinTable();

	//
	// Members.
	//
	int		m_nTable;		// The table to join onto.
	int		m_nLHSColumn;	// The left hand tables' column.
	int		m_nType;		// The join type (INNER or OUTER).
	int		m_nRHSColumn;	// The right hand tables' column.
};

/******************************************************************************
** 
** This class is used to hold the query when doing a SELECT involving joins.
**
*******************************************************************************
*/

class CJoin : protected TPtrArray<CJoinTable>
{
public:
	//
	// Constructors/Destructor.
	//
	CJoin(int nTable);
	~CJoin();

	// Join types
	enum Type
	{
		INNER,
		OUTER,
	};
	
	//
	// Methods.
	//
	int         Count() const;
	CJoinTable& Join(int n) const;
	CJoinTable& operator[](int n) const;

	void Add(int nTable, int nLHSColumn, Type eType, int nRHSColumn);

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

inline CJoin::CJoin(int nTable)
{
	TPtrArray<CJoinTable>::Add(new CJoinTable(nTable, -1, -1, -1));
}

inline CJoin::~CJoin()
{
	TPtrArray<CJoinTable>::DeleteAll();
}

inline int CJoin::Count() const
{
	return Size();
}

inline CJoinTable& CJoin::Join(int n) const
{
	ASSERT((n >= 0) && (n < Count()));

	return *(TPtrArray<CJoinTable>::At(n));
}

inline CJoinTable& CJoin::operator[](int n) const
{
	ASSERT((n >= 0) && (n < Count()));

	return *(TPtrArray<CJoinTable>::At(n));
}

inline void CJoin::Add(int nTable, int nLHSColumn, Type eType, int nRHSColumn)
{
	TPtrArray<CJoinTable>::Add(new CJoinTable(nTable, nLHSColumn, eType, nRHSColumn));
}

inline CJoinTable::CJoinTable(int nTable, int nLHSColumn, int nType, int nRHSColumn)
	: m_nTable(nTable)
	, m_nLHSColumn(nLHSColumn)
	, m_nType(nType)
	, m_nRHSColumn(nRHSColumn)
{
}

inline CJoinTable::~CJoinTable()
{
}

#endif //JOIN_HPP
