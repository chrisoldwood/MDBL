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

#include <vector>

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
	std::vector<size_t>		m_aiColumns;	// The list of columns.
	std::vector<Dir>		m_aiSortDirs;	// The list of column sort directions.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CSortColumns::CSortColumns()
	: m_aiColumns()
	, m_aiSortDirs()
{
}

inline CSortColumns::CSortColumns(size_t nColumn, Dir eDir)
	: m_aiColumns()
	, m_aiSortDirs()
{
	Add(nColumn, eDir);
}

inline CSortColumns::~CSortColumns()
{
}

inline size_t CSortColumns::Count() const
{
	return m_aiColumns.size();
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
	m_aiColumns.push_back(nColumn);
	m_aiSortDirs.push_back(eDir);
}

#endif //SORTCOLUMNS_HPP
