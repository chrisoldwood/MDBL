/******************************************************************************
**
** MODULE:		FIELD.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CField class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FIELD_HPP
#define FIELD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "FwdDecls.hpp"
#include "Value.hpp"

/******************************************************************************
**
** The class that is used to store the value for a rows' column.
**
*******************************************************************************
*/

class CField
{
public:
	//
	// Parents.
	//
	CRow&    Row() const;
	CColumn& Column() const;

	//
	// Accessors.
	//
	int               GetInt()       const;
	int64             GetInt64()     const;
	double            GetDouble()    const;
	tchar             GetChar()      const;
	const tchar*      GetString()    const;
	bool              GetBool()      const;
	time_t	          GetDateTime()  const;
	const CTimeStamp& GetTimeStamp() const;
	CValue            GetValue()     const;
	void*             GetPtr()       const;
	CRow*             GetRowPtr()    const;
	CRow**            GetRowSetPtr() const;
	void              GetRaw(void* pValue) const;

	//
	// Mutators.
	//
	void SetNull();
	void SetInt(int iValue);
	void SetInt64(int64 iValue);
	void SetDouble(double dValue);
	void SetChar(tchar cValue);
	void SetString(const tchar* sValue);
	void SetBool(bool bValue);
	void SetDateTime(time_t tValue);
	void SetTimeStamp(const CTimeStamp& tsValue);
	void SetField(const CField& oValue);
	void SetPtr(void* pValue);
	void SetRowPtr(CRow* pValue);
	void SetRowSetPtr(CRow** pValue);
	void SetRaw(const void* pValue);

	//
	// Conversion operators.
	//
	operator int()               const;
	operator double()            const;
	operator tchar()             const;
	operator const tchar*()      const;
	operator bool()              const;
	operator time_t()            const;
	operator const CTimeStamp&() const;
	operator CValue()            const;
	operator void*()             const;
	operator CRow*()             const;
	operator CRow**()            const;


	CField& operator=(const CNull& oNull);
	CField& operator=(int iValue);
	CField& operator=(double dValue);
	CField& operator=(tchar cValue);
	CField& operator=(const tchar* sValue);
	CField& operator=(bool bValue);
	CField& operator=(time_t tValue);
	CField& operator=(const CTimeStamp& tsValue);
	CField& operator=(const CField& oValue);
	CField& operator=(void* pValue);
	CField& operator=(CRow* pValue);
	CField& operator=(CRow** pValue);

	//
	// Comparison operators.
	//
	bool operator==(const CNull& oNull) const;
	bool operator!=(const CNull& oNull) const;
	bool operator==(int  iValue) const;
	bool operator!=(int  iValue) const;
	bool operator==(double dValue) const;
	bool operator!=(double dValue) const;
	bool operator==(tchar cValue) const;
	bool operator!=(tchar cValue) const;
	bool operator==(const tchar* sValue) const;
	bool operator!=(const tchar* sValue) const;
	bool operator==(bool bValue) const;
	bool operator!=(bool bValue) const;
	bool operator==(const CValue& oValue) const;
	bool operator!=(const CValue& oValue) const;
	bool operator==(const CField& oValue) const;
	bool operator!=(const CField& oValue) const;

	int Compare(const CField& oValue) const;
	int Compare(const CValue& oValue) const;

	//
	// Persistance methods.
	//
	bool Modified() const;

	//
	// String formatting methods.
	//
	CString Format(const tchar* pszFormat = NULL) const;
	CString DbgFormat() const;

protected:
	//
	// Members.
	//
	CRow&		m_oRow;			// The parent row.
	CColumn&	m_oColumn;		// The parent column.
	size_t		m_nColumn;		// The parent column index.
	bool		m_bModified;	// Modified flag.
	bool		m_bNull;		// NULL flag.
union
{
	int*		m_pInt;			// Pointer to value if type is MDCT_INT or MDCT_IDENTITY.
	int64*		m_pInt64;		// Pointer to value if type is MDCT_INT64.
	double*		m_pDouble;		// Pointer to value if type is MDCT_DOUBLE.
	tchar*		m_pChar;		// Pointer to value if type is MDCT_CHAR.
	tchar*		m_pString;		// Pointer to value if type is MDCT_FXDSTR or MDCT_VARSTR.
	bool*		m_pBool;		// Pointer to value if type is MDCT_BOOL.
	time_t*		m_pTimeT;		// Pointer to value if type is MDCT_DATETIME or MDCT_DATE or MDCT_TIME.
	CTimeStamp*	m_pTimeStamp;	// Pointer to value if type is MDCT_TIMESTAMP.
	void*		m_pVoidPtr;		// Value if type is MDCT_VOIDPTR.
	CRow*		m_pRowPtr;		// Value if type is MDCT_ROWPTR.
	CRow**		m_pRowSetPtr;	// Value if type is MDCT_ROWSETPTR.
};

	//
	// Friends.
	//
	friend class CRow;

private:
	//
	// Only allow CRow to create and destroy.
	//
	CField(CRow& oRow, CColumn& oColumn, size_t nColumn, bool bNull, void* pData);
	~CField();

#pragma push_macro("new")
#undef new

	//
	// Placement new/delete operators.
	//
	void* operator new(size_t n, void* p);
	void  operator delete(void* p, void* p2);
	void  operator delete(void* p);

#pragma pop_macro("new")

	//
	// Disallow copies for now.
	//
	CField(const CField& oValue);

	//
	// Internal methods.
	//
	void    Updated();
	CString FormatTimeT(const tchar* pszFormat) const;
	CString FormatTimeStamp(const tchar* pszFormat) const;
	CString FormatBool(const tchar* pszFormat) const;
	int     StrCmp(const tchar* pszRHS) const;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CRow& CField::Row() const
{
	return m_oRow;
}

inline CColumn& CField::Column() const
{
	return m_oColumn;
}

#pragma push_macro("new")
#undef new

inline void* CField::operator new(size_t, void* p)
{
	return p;
}

inline void  CField::operator delete(void*, void*)
{
}

inline void  CField::operator delete(void*)
{
}

#pragma pop_macro("new")

inline CField::operator int() const
{
	return GetInt();
}

inline CField::operator double() const
{
	return GetDouble();
}

inline CField::operator tchar() const
{
	return GetChar();
}

inline CField::operator const tchar*() const
{
	return GetString();
}

inline CField::operator bool() const
{
	return GetBool();
}

inline CField::operator time_t() const
{
	return GetDateTime();
}

inline CField::operator const CTimeStamp&() const
{
	return GetTimeStamp();
}

inline CField::operator CValue() const
{
	return GetValue();
}

inline CField::operator void*() const
{
	return GetPtr();
}

inline CField::operator CRow*() const
{
	return GetRowPtr();
}

inline CField::operator CRow**() const
{
	return GetRowSetPtr();
}

inline CField& CField::operator=(const CNull&)
{
	SetNull();

	return *this;
}

inline CField& CField::operator=(int iValue)
{
	SetInt(iValue);

	return *this;
}

inline CField& CField::operator=(double dValue)
{
	SetDouble(dValue);

	return *this;
}

inline CField& CField::operator=(tchar cValue)
{
	SetChar(cValue);

	return *this;
}

inline CField& CField::operator=(const tchar* sValue)
{
	SetString(sValue);

	return *this;
}

inline CField& CField::operator=(bool bValue)
{
	SetBool(bValue);

	return *this;
}

inline CField& CField::operator=(time_t tValue)
{
	SetDateTime(tValue);

	return *this;
}

inline CField& CField::operator=(const CTimeStamp& tsValue)
{
	SetTimeStamp(tsValue);

	return *this;
}

inline CField& CField::operator=(const CField& oValue)
{
	SetField(oValue);

	return *this;
}

inline CField& CField::operator=(void* pValue)
{
	SetPtr(pValue);

	return *this;
}

inline CField& CField::operator=(CRow* pValue)
{
	SetRowPtr(pValue);

	return *this;
}

inline CField& CField::operator=(CRow** pValue)
{
	SetRowSetPtr(pValue);

	return *this;
}

inline bool CField::operator==(const CNull&) const
{
	return m_bNull;
}

inline bool CField::operator!=(const CNull&) const
{
	return !m_bNull;
}

inline bool CField::operator==(int iValue) const
{
	return (iValue == GetInt());
}

inline bool CField::operator!=(int iValue) const
{
	return (iValue != GetInt());
}

inline bool CField::operator==(double dValue) const
{
	return (dValue == GetDouble());
}

inline bool CField::operator!=(double dValue) const
{
	return (dValue != GetDouble());
}

inline bool CField::operator==(tchar cValue) const
{
	return (cValue == GetChar());
}

inline bool CField::operator!=(tchar cValue) const
{
	return (cValue != GetChar());
}

inline bool CField::operator==(const tchar* sValue) const
{
	return (tstrcmp(sValue, GetString()) == 0);
}

inline bool CField::operator!=(const tchar* sValue) const
{
	return (tstrcmp(sValue, GetString()) != 0);
}

inline bool CField::operator==(bool bValue) const
{
	return (bValue == GetBool());
}

inline bool CField::operator!=(bool bValue) const
{
	return (bValue != GetBool());
}

inline bool CField::operator!=(const CValue& oValue) const
{
	return !operator==(oValue);
}

inline bool CField::operator==(const CField& oValue) const
{
	return operator==(oValue.GetValue());
}

inline bool CField::operator!=(const CField& oValue) const
{
	return !operator==(oValue);
}

inline bool CField::Modified() const
{
	return m_bModified;
}

#endif //FIELD_HPP
