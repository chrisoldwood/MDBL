/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		JOINEDSET.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CJoinedSet class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef JOINEDSET_HPP
#define JOINEDSET_HPP

/******************************************************************************
** 
** This is a 2D array based class used to store the results of a join query.
**
*******************************************************************************
*/

class CJoinedSet : public CPtrArray
{
public:
	//
	// Constructors/Destructor.
	//
	CJoinedSet(int nTables);
	~CJoinedSet();
	
protected:
	//
	// Members.
	//
	int m_nTables;	// Number of joined tables.

private:
	//
	// Disallow copies/assignment for now.
	//
//	CJoinedSet(const CJoinedSet& oJoinedSet);
	void operator=(const CJoinedSet&);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //JOINEDSET_HPP
