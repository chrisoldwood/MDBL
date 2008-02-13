/******************************************************************************
**
** MODULE:		VALUE.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CValue class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef VALUE_HPP
#define VALUE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "MDBLTypes.hpp"

/******************************************************************************
** 
** This class is a VARIANT like class used to hold values of different types.
**
*******************************************************************************
*/

class CValue
{
public:
	//
	// Constructors/Destructor.
	//
	CValue(const CNull& oNull);
	CValue(int iValue);
	CValue(double dValue);
	CValue(tchar cValue);
	CValue(const tchar* sValue);
	CValue(const CString& sValue);
	CValue(bool bValue);
	CValue(time_t tValue);
	CValue(void* pValue);
	~CValue();

	CValue(const CValue& oValue);
	const CValue& operator=(const CValue& oValue);
	
	//
	// Members.
	//
	STGTYPE	m_eType;		// Data type.
	bool	m_bNull;		// Value if type is MDST_NULL.

union
{
	int		m_iValue;		// Value if type is MDST_INT
	double	m_dValue;		// Value if type is MDST_DOUBLE.
	tchar	m_cValue;		// Value if type is MDST_CHAR.
	tchar*	m_sValue;		// Value if type is MDST_STRING
	bool	m_bValue;		// Value if type is MDST_BOOL.
	time_t	m_tValue;		// Value if type is MDST_TIME_T.
	void*	m_pValue;		// Value if type is MDST_POINTER.
	byte	m_uValue[8];	// Member used when copying.
};

};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CValue::CValue(const CNull&)
	: m_eType(MDST_NULL)
	, m_bNull(true)
{
}

inline CValue::CValue(int iValue)
	: m_eType(MDST_INT)
	, m_bNull(false)
	, m_iValue(iValue)
{
}

inline CValue::CValue(double dValue)
	: m_eType(MDST_DOUBLE)
	, m_bNull(false)
	, m_dValue(dValue)
{
}

inline CValue::CValue(tchar cValue)
	: m_eType(MDST_CHAR)
	, m_bNull(false)
	, m_cValue(cValue)
{
}

inline CValue::CValue(const tchar* sValue)
	: m_eType(MDST_STRING)
	, m_bNull(false)
	, m_sValue(new tchar[tstrlen(sValue)+1])
{
	tstrcpy(m_sValue, sValue);
}

inline CValue::CValue(const CString& sValue)
	: m_eType(MDST_STRING)
	, m_bNull(false)
	, m_sValue(new tchar[tstrlen(sValue)+1])
{
	tstrcpy(m_sValue, sValue);
}

inline CValue::CValue(bool bValue)
	: m_eType(MDST_BOOL)
	, m_bNull(false)
	, m_bValue(bValue)
{
}

inline CValue::CValue(time_t tValue)
	: m_eType(MDST_TIME_T)
	, m_bNull(false)
	, m_tValue(tValue)
{
}

inline CValue::CValue(void* pValue)
	: m_eType(MDST_POINTER)
	, m_bNull(false)
	, m_pValue(pValue)
{
}

inline CValue::CValue(const CValue& oValue)
	: m_eType(oValue.m_eType)
	, m_bNull(oValue.m_bNull)
{
	if (m_eType != MDST_STRING)
	{
		memcpy(m_uValue, oValue.m_uValue, sizeof(m_uValue));
	}
	else
	{
		m_sValue = new tchar[tstrlen(oValue.m_sValue)+1];
		tstrcpy(m_sValue, oValue.m_sValue);
	}
}

inline const CValue& CValue::operator=(const CValue& oValue)
{
	m_eType = oValue.m_eType;
	m_bNull = oValue.m_bNull;

	if (m_eType != MDST_STRING)
	{
		memcpy(m_uValue, oValue.m_uValue, sizeof(m_uValue));
	}
	else
	{
		m_sValue = new tchar[tstrlen(oValue.m_sValue)+1];
		tstrcpy(m_sValue, oValue.m_sValue);
	}

	return *this;
}

inline CValue::~CValue()
{
	if (m_eType == MDST_STRING)
		delete[] m_sValue;
}

#endif //VALUE_HPP
