/******************************************************************************
** (C) Chris Oldwood
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
	virtual CJoinedSet Select(const CJoin& oJoin) const;

	//
	// Persistance methods.
	//
	virtual bool Modified() const;

	virtual void operator <<(CStream& rStream);
	virtual void operator >>(CStream& rStream);

	virtual void operator <<(CSQLSource& rSource);
	virtual void operator >>(CSQLSource& rSource);

protected:
	//
	// Members.
	//
	CTableSet	m_vTables;		// The tables.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline int CMDB::TableCount() const
{
	return m_vTables.Size();
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
