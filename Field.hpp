/******************************************************************************
** (C) Chris Oldwood
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

	void operator=(const CNull& oNull);
	void operator=(int iValue);
	void operator=(double dValue);
	void operator=(char cValue);
	void operator=(const char* sValue);
	void operator=(bool bValue);
	void operator=(time_t tValue);
	void operator=(const CTimeStamp& tsValue);

	//
	// Comparison operators.
	//
	bool operator==(const CNull& oNull) const;
	bool operator!=(const CNull& oNull) const;
	bool operator==(const CValue& oValue) const;
	bool operator!=(const CValue& oValue) const;

protected:
	//
	// Members.
	//
	CRow*		m_pRow;			// The parent row.
	CColumn*	m_pColumn;		// The parent column.
	bool		m_bNull;		// NULL flag.
union
{
	void*		m_pVoid;		// Generic pointer to value.
	int*		m_pInt;			// Pointer to value if type is MDCT_INT or MDCT_IDENTITY.
	double*		m_pDouble;		// Pointer to value if type is MDCT_DOUBLE.
	char*		m_pChar;		// Pointer to value if type is MDCT_CHAR.
	char*		m_pString;		// Pointer to value if type is MDCT_FXDSTR or MDCT_VARSTR.
	bool*		m_pBool;		// Pointer to value if type is MDCT_BOOL.
	time_t*		m_pTimeT;		// Pointer to value if type is MDCT_DATETIME.
	CTimeStamp*	m_pTimeStamp;	// Pointer to value if type is MDCT_TIMESTAMP.
};

	//
	// Friends.
	//
	friend class CRow;

private:
	//
	// Only allow CRow to create and destroy.
	//
	CField();
	~CField();

	//
	// Disallow copies & assignments for now.
	//
	CField(const CField& oValue);
	CField& operator=(const CField& oValue);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

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

inline bool CField::operator==(const CNull& oNull) const
{
	return m_bNull;
}

inline bool CField::operator!=(const CNull& oNull) const
{
	return !m_bNull;
}

inline bool CField::operator!=(const CValue& oValue) const
{
	return !operator==(oValue);
}

#endif //FIELD_HPP
