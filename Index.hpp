/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		INDEX.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CIndex class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef INDEX_HPP
#define INDEX_HPP

// Forward declarations.
class CTable;

/******************************************************************************
** 
** This is the base class for all index classes.
**
*******************************************************************************
*/

class CIndex
{
public:
	//
	// Methods.
	//
	int Column() const;

	virtual void AddRow(CRow& oRow) = 0;
	virtual void RemoveRow(CRow& oRow) = 0;
	virtual void Truncate() = 0;

	virtual CRow* FindRow(const CValue& oValue) const = 0;

	//
	// Index types.
	//
	enum Type
	{
		MAP  = 0,	// Map based for very unique data.
		TREE = 1,	// Tree based for less unique data.
	};

protected:
	//
	// Constructors/Destructor.
	//
	CIndex(CTable& oTable, int nColumn, bool bUnique);
	virtual ~CIndex();
	
	//
	// Members.
	//
	CTable&	m_oTable;		// The parent table.
	int		m_nColumn;		// The column to be indexed.
	bool	m_bUnique;		// Only accept unique values.

	//
	// Friends.
	//
	friend class CColumn;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline int CIndex::Column() const
{
	return m_nColumn;
}

#endif //INDEX_HPP
