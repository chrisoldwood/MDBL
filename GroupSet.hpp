/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		GROUPSET.HPP
** COMPONENT:	Memory Database Library
** DESCRIPTION:	The CGroupSet class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef GROUPSET_HPP
#define GROUPSET_HPP

/******************************************************************************
** 
** This is a 2D array based class used to store the results of a GROUP BY.
**
*******************************************************************************
*/

class CGroupSet
{
public:
	//
	// Constructors/Destructor.
	//
	CGroupSet();
	CGroupSet(const CGroupSet& oSet);
	~CGroupSet();
	
	//
	// Methods.
	//
	int         Count() const;
	CResultSet& ResultSet(int n) const;
	CResultSet& operator[](int n) const;

	void Add(const CResultSet& oRS);

protected:
	// Template shorthands.
	typedef TPtrArray<CResultSet> CResultSets;

	//
	// Members.
	//
	CResultSets	m_oResSets;

private:
	//
	// Disallow assignment.
	//
	void operator=(const CGroupSet&);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline int CGroupSet::Count() const
{
	return m_oResSets.Size();
}

inline CResultSet& CGroupSet::ResultSet(int n) const
{
	return *m_oResSets[n];
}

inline CResultSet& CGroupSet::operator[](int n) const
{
	return *m_oResSets[n];
}

inline void CGroupSet::Add(const CResultSet& oRS)
{
	m_oResSets.Add(new CResultSet(oRS));
}

#endif // GROUPSET_HPP
