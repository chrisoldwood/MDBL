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
	CSortColumns(int nColumn, Dir eDir);
	~CSortColumns();

	//
	// Methods.
	//
	int  Count() const;
	int	 Column(int n) const;
	Dir  Direction(int n) const;

	void Add(int nColumn, Dir eDir);

protected:
	//
	// Members.
	//
	TArray<int>	m_aiColumns;	// The list of columns.
	TArray<int>	m_aiSortDirs;	// The list of column sort directions.
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

inline CSortColumns::CSortColumns(int nColumn, Dir eDir)
{
	Add(nColumn, eDir);
}

inline CSortColumns::~CSortColumns()
{
}

inline int CSortColumns::Count() const
{
	return m_aiColumns.Size();
}

inline int CSortColumns::Column(int n) const
{
	return m_aiColumns[n];
}

inline CSortColumns::Dir CSortColumns::Direction(int n) const
{
	return static_cast<Dir>(m_aiSortDirs[n]);
}

inline void CSortColumns::Add(int nColumn, Dir eDir)
{
	m_aiColumns.Add(nColumn);
	m_aiSortDirs.Add(eDir);
}

#endif //SORTCOLUMNS_HPP
