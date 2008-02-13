/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SORTCOLUMNS.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CSortColumns class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SORTCOLUMNS_HPP
#define SORTCOLUMNS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <Legacy/TArray.hpp>

/******************************************************************************
** 
** This class holds the list of columns used to sort a result set.
**
*******************************************************************************
*/

class CSortColumns
{
public:
	// Sort directions.
	enum Dir
	{
		DESC = -1,
		ASC  = +1,
	};

	//
	// Constructors/Destructor.
	//
	CSortColumns();
	CSortColumns(size_t nColumn, Dir eDir);
	~CSortColumns();

	//
	// Methods.
	//
	size_t Count() const;
	size_t Column(size_t n) const;
	Dir  Direction(size_t n) const;

	void Add(size_t nColumn, Dir eDir);

protected:
	//
	// Members.
	//
	TArray<size_t>	m_aiColumns;	// The list of columns.
	TArray<Dir>		m_aiSortDirs;	// The list of column sort directions.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CSortColumns::CSortColumns()
{
}

inline CSortColumns::CSortColumns(size_t nColumn, Dir eDir)
{
	Add(nColumn, eDir);
}

inline CSortColumns::~CSortColumns()
{
}

inline size_t CSortColumns::Count() const
{
	return m_aiColumns.Size();
}

inline size_t CSortColumns::Column(size_t n) const
{
	return m_aiColumns[n];
}

inline CSortColumns::Dir CSortColumns::Direction(size_t n) const
{
	return static_cast<Dir>(m_aiSortDirs[n]);
}

inline void CSortColumns::Add(size_t nColumn, Dir eDir)
{
	m_aiColumns.Add(nColumn);
	m_aiSortDirs.Add(eDir);
}

#endif //SORTCOLUMNS_HPP
