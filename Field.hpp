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

// Forward declarations.
class CRow;
class CColumn;

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
	double            GetDouble()    const;
	char              GetChar()      const;
	const char*       GetString()    const;
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
	void SetDouble(double dValue);
	void SetChar(char cValue);
	void SetString(const char* sValue);
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
	operator char()              const;
	operator const char*()       const;
	operator bool()              const;
	operator time_t()            const;
	operator const CTimeStamp&() const;
	operator CValue()            const;
	operator void*()             const;
	operator CRow*()             const;
	operator CRow**()            const;
	

	void operator=(const CNull& oNull);
	void operator=(int iValue);
	void operator=(double dValue);
	void operator=(char cValue);
	void operator=(const char* sValue);
	void operator=(bool bValue);
	void operator=(time_t tValue);
	void operator=(const CTimeStamp& tsValue);
	void operator=(const CField& oValue);
	void operator=(void* pValue);
	void operator=(CRow* pValue);
	void operator=(CRow** pValue);

	//
	// Comparison operators.
	//
	bool operator==(const CNull& oNull) const;
	bool operator!=(const CNull& oNull) const;
	bool operator==(int  iValue) const;
	bool operator!=(int  iValue) const;
	bool operator==(double dValue) const;
	bool operator!=(double dValue) const;
	bool operator==(char cValue) const;
	bool operator!=(char cValue) const;
	bool operator==(const char* sValue) const;
	bool operator!=(const char* sValue) const;
	bool operator==(bool bValue) const;
	bool operator!=(bool bValue) const;
	bool operator==(const CValue& oValue) const;
	bool operator!=(const CValue& oValue) const;
	bool operator==(const CField& oValue) const;
	bool operator!=(const CField& oValue) const;

	int Compare(const CField& oValue) const;

	//
	// Persistance methods.
	//
	bool Modified() const;

	//
	// String formatting methods.
	//
	CString Format(const char* pszFormat = NULL) const;
	CString DbgFormat() const;

protected:
	//
	// Members.
	//
	CRow&		m_oRow;			// The parent row.
	CColumn&	m_oColumn;		// The parent column.
	int			m_nColumn;		// The parent column index.
	bool		m_bModified;	// Modified flag.
	bool		m_bNull;		// NULL flag.
union
{
	int*		m_pInt;			// Pointer to value if type is MDCT_INT or MDCT_IDENTITY.
	double*		m_pDouble;		// Pointer to value if type is MDCT_DOUBLE.
	char*		m_pChar;		// Pointer to value if type is MDCT_CHAR.
	char*		m_pString;		// Pointer to value if type is MDCT_FXDSTR or MDCT_VARSTR.
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
	CField(CRow& oRow, CColumn& oColumn, int nColumn, bool bNull, void* pData);
	~CField();

	//
	// Placement new/delete operators.
	//
	void* operator new(size_t n, void* p);
	void  operator delete(void* p, void* p2);
	void  operator delete(void* p);

	//
	// Disallow copies for now.
	//
	CField(const CField& oValue);

	//
	// Internal methods.
	//
	void    Updated();
	CString FormatTimeT(const char* pszFormat) const;
	CString FormatBool(const char* pszFormat) const;
	int     StrCmp(const char* pszRHS) const;
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

inline CField::operator int() const
{
	return GetInt();
}

inline CField::operator double() const
{
	return GetDouble();
}

inline CField::operator char() const
{
	return GetChar();
}

inline CField::operator const char*() const
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

inline void CField::operator=(const CNull&)
{
	SetNull();
}

inline void CField::operator=(int iValue)
{
	SetInt(iValue);
}

inline void CField::operator=(double dValue)
{
	SetDouble(dValue);
}

inline void CField::operator=(char cValue)
{
	SetChar(cValue);
}

inline void CField::operator=(const char* sValue)
{
	SetString(sValue);
}

inline void CField::operator=(bool bValue)
{
	SetBool(bValue);
}

inline void CField::operator=(time_t tValue)
{
	SetDateTime(tValue);
}

inline void CField::operator=(const CTimeStamp& tsValue)
{
	SetTimeStamp(tsValue);
}

inline void CField::operator=(const CField& oValue)
{
	SetField(oValue);
}

inline void CField::operator=(void* pValue)
{
	SetPtr(pValue);
}

inline void CField::operator=(CRow* pValue)
{
	SetRowPtr(pValue);
}

inline void CField::operator=(CRow** pValue)
{
	SetRowSetPtr(pValue);
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

inline bool CField::operator==(char cValue) const
{
	return (cValue == GetChar());
}

inline bool CField::operator!=(char cValue) const
{
	return (cValue != GetChar());
}

inline bool CField::operator==(const char* sValue) const
{
	return (strcmp(sValue, GetString()) == 0);
}

inline bool CField::operator!=(const char* sValue) const
{
	return (strcmp(sValue, GetString()) != 0);
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
