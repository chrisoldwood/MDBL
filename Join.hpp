/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		JOIN.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CJoin class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef JOIN_HPP
#define JOIN_HPP

/******************************************************************************
** 
** This class is used to hold for joining two tables in a SELECT.
**
*******************************************************************************
*/

class CJoin
{
public:
	//
	// Constructors/Destructor.
	//
	CJoin(int nLHSTable, int nLHSColumn, int nRHSTable, int nRHSColumn);
	~CJoin();
	
protected:
	//
	// Members.
	//
	int m_nLHSTable;	// Left-hand side table.
	int m_nLHSColumn;	// Left-hand side table column.
	int m_nRHSTable;	// Right-hand side table.
	int m_nRHSColumn;	// Right-hand side table column.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //JOIN_HPP
