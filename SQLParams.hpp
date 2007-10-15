/******************************************************************************
**
** MODULE:		SQLPARAMS.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CSQLParams class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SQLPARAMS_HPP
#define SQLPARAMS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "MDBLTypes.hpp"

// Forward declarations.
class CRow;

/******************************************************************************
** 
** The type used to hold column information.
**
*******************************************************************************
*/

struct SQLParam
{
	int		m_nSrcColumn;	// The source column.
	COLTYPE	m_eMDBColType;	// The MDB column type.
	int		m_nMDBColSize;	// The MDB column size.
	int		m_nBufType;		// The input buffer type.
	int		m_nBufSize;		// The input buffer length.
	int		m_nSQLColSize;	// The SQL column size.
};

/******************************************************************************
** 
** This is the base class for SQL statment parameters.
**
*******************************************************************************
*/

class CSQLParams
{
public:
	//
	// Constructors/Destructor.
	//
	CSQLParams();
	virtual ~CSQLParams();
	
	//
	// Accessors.
	//
	virtual int NumParams() const = 0;
	virtual SQLParam& Param(int n) const = 0;

	virtual void SetRow(CRow& oRow) = 0;

protected:
	//
	// Members.
	//
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CSQLParams::CSQLParams()
{
}

inline CSQLParams::~CSQLParams()
{
}

#endif //SQLPARAMS_HPP
