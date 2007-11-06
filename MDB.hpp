/******************************************************************************
**
** MODULE:		MDB.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CMDB class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef MDB_HPP
#define MDB_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Table.hpp"
#include "TableSet.hpp"

/******************************************************************************
** 
** This is the class used (possibly as a base class) for all Memory Databases.
**
*******************************************************************************
*/

class CMDB
{
public:
	//
	// Constructors/Destructor.
	//
	CMDB();
	virtual ~CMDB();
	
	//
	// Table methods.
	//
	virtual int     TableCount() const;
	virtual CTable& Table(int n) const;
	virtual CTable& operator[](int n) const;

	virtual CTable& CreateTable(const char* pszName);
	virtual CTable& CreateTable(const char* pszName, CSQLSource& oConnection, const char* pszQuery = NULL);
	virtual int     AddTable(CTable& oTable);
	virtual int     FindTable(const char* pszName);

	//
	// Query methods.
	//
	virtual CJoinedSet Select(const CJoin& oQuery) const;

	//
	// Persistance methods.
	//
	virtual bool Modified() const;

	virtual void Read (WCL::IInputStream&  rStream);
	virtual void Write(WCL::IOutputStream& rStream);

	virtual void Read(CSQLSource& rSource);
	virtual void Write(CSQLSource& rSource, CTable::RowTypes eRows = CTable::ALL);

	virtual void ResetRowFlags();

	//
	// Debug methods.
	//
	virtual void Dump(WCL::IOutputStream& rStream) const;

protected:
	//
	// Members.
	//
	CTableSet	m_vTables;		// The tables.

	//
	// Internal methods.
	//
	int DoJoin(const CJoin& oQuery, int nJoin, const CRow& oLHSRow, CJoinedSet& oJS) const;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline int CMDB::TableCount() const
{
	return m_vTables.Count();
}

inline CTable& CMDB::Table(int n) const
{
	return m_vTables.Table(n);
}

inline CTable& CMDB::operator[](int n) const
{
	return m_vTables.Table(n);
}

#endif //MDB_HPP
