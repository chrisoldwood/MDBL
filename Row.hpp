/******************************************************************************
**
** MODULE:		ROW.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CRow class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef ROW_HPP
#define ROW_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Field.hpp"

/******************************************************************************
** 
** The class used to store a row of data in a table.
**
*******************************************************************************
*/

class CRow
{
public:
	//
	// Constructors/Destructor.
	//
	CRow(CTable& oTable, bool bNull = false);
	~CRow();
	
	//
	// Accessors & mutators.
	//
	CField& Field(size_t n) const;
	CField& operator[](size_t n) const;

	CTable& Table() const;

	uint Status() const;
	bool InTable() const;
	bool Inserted() const;
	bool Updated() const;
	bool Deleted() const;

	void ResetStatus();
	void MarkOriginal();
	void MarkInserted();
	void MarkUpdated();
	void MarkDeleted();

	//
	// Persistance methods.
	//
	bool Modified() const;

	void Read (WCL::IInputStream&  rStream);
	void Write(WCL::IOutputStream& rStream);

	//
	// Row status flags.
	//
	enum StatusFlags
	{
		ALLOCATED = 0x00,
		ORIGINAL  = 0x01,
		INSERTED  = 0x02,
		UPDATED   = 0x04,
		DELETED   = 0x08,
	};

protected:
	//
	// Members.
	//
	CTable&	m_oTable;		// The parent table.
	CField*	m_aFields;		// The data fields.
	size_t	m_nColumns;		// The number of fields.
	uint	m_eStatus;		// The status.

	//
	// Friends.
	//

private:
	//
	// Disallow copies & assignments for now.
	//
	CRow(const CRow& oRow);
	CRow& operator=(const CRow& oRow);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CField& CRow::Field(size_t n) const
{
	ASSERT(n < m_nColumns);

	return m_aFields[n];
}

inline CField& CRow::operator[](size_t n) const
{
	ASSERT(n < m_nColumns);

	return m_aFields[n];
}

inline CTable& CRow::Table() const
{
	return m_oTable;
}

inline uint CRow::Status() const
{
	return m_eStatus;
}

inline bool CRow::InTable() const
{
	return (m_eStatus != ALLOCATED);
}

inline bool CRow::Inserted() const
{
	return (m_eStatus & INSERTED);
}

inline bool CRow::Updated() const
{
	return (m_eStatus & UPDATED);
}

inline bool CRow::Deleted() const
{
	return (m_eStatus & DELETED);
}

inline void CRow::ResetStatus()
{
	m_eStatus = ORIGINAL;

	for (size_t i = 0; i < m_nColumns; ++i)
		m_aFields[i].m_bModified = false;
}

inline void CRow::MarkOriginal()
{
	m_eStatus |= ORIGINAL;
}

inline void CRow::MarkInserted()
{
	m_eStatus |= INSERTED;
}

inline void CRow::MarkUpdated()
{
	m_eStatus |= UPDATED;
}

inline void CRow::MarkDeleted()
{
	m_eStatus |= DELETED;
}

inline bool CRow::Modified() const
{
	return ( (m_eStatus != ALLOCATED) && (m_eStatus != ORIGINAL) );
}

#endif //ROW_HPP
