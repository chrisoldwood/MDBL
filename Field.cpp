/******************************************************************************
**
** MODULE:		FIELD.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CField class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "Field.hpp"
#include "Column.hpp"
#include "Row.hpp"
#include "Table.hpp"
#include "TimeStamp.hpp"
#include <time.h>

// The special null value.
CNull null;

// Default format specifiers.
static const char* pszFormats[] = 
{
	"%d",					// MDCT_INT
	"%.2f",					// MDCT_DOUBLE
	"%c",					// MDCT_CHAR
	"%s",					// MDCT_FXDSTR
	"%s",					// MDCT_VARSTR
	"No|Yes",				// MDCT_BOOL
	"%d",					// MDCT_IDENTITY
	"%d/%m/%y %H:%M:%S",	// MDCT_DATETIME
	"%d/%m/%y",				// MDCT_DATE
	"%H:%M:%S",				// MDCT_TIME
	"%d/%m/%y %H:%M:%S",	// MDCT_TIMESTAMP
	"%p",					// MDCT_VOIDPTR
	"%p",					// MDCT_ROWPTR
	"%p"					// MDCT_ROWSETPTR
};

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CField::CField(CRow& oRow, CColumn& oColumn, int nColumn, bool bNull, void* pData)
	: m_oRow(oRow)
	, m_oColumn(oColumn)
	, m_nColumn(nColumn)
	, m_bModified(false)
	, m_bNull(bNull)
	, m_pVoidPtr(pData)
{
	// POINTER fields store their values 'in-place'.
	if (m_oColumn.StgType() == MDST_POINTER)
		m_pVoidPtr = NULL;

	// VARSTR fields store their values in a separate buffer.
	if (m_oColumn.ColType() == MDCT_VARSTR)
		m_pString = (char*) calloc(1, sizeof(char));
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CField::~CField()
{
	// VARSTR fields store their values in a separate block.
	if (m_oColumn.ColType() == MDCT_VARSTR)
		free(m_pString);
}

/******************************************************************************
** Method:		Get"Type"()
**
** Description:	Returns the value of the requested type.
**
** Parameters:	None.
**
** Returns:		The value.
**
*******************************************************************************
*/

int CField::GetInt() const
{
	ASSERT(m_bNull   != true);
	ASSERT(m_oColumn.StgType() == MDST_INT);

	return *m_pInt;
}

double CField::GetDouble() const
{
	ASSERT(m_bNull   != true);
	ASSERT(m_oColumn.StgType() == MDST_DOUBLE);

	return *m_pDouble;
}

char CField::GetChar() const
{
	ASSERT(m_bNull   != true);
	ASSERT(m_oColumn.StgType() == MDST_CHAR);

	return *m_pChar;
}

const char* CField::GetString() const
{
	ASSERT(m_bNull   != true);
	ASSERT(m_oColumn.StgType() == MDST_STRING);

	return m_pString;
}

bool CField::GetBool() const
{
	ASSERT(m_bNull   != true);
	ASSERT(m_oColumn.StgType() == MDST_BOOL);

	return *m_pBool;
}

time_t CField::GetDateTime() const
{
	ASSERT(m_bNull   != true);
	ASSERT(m_oColumn.StgType() == MDST_TIME_T);

	return *m_pTimeT;
}

const CTimeStamp& CField::GetTimeStamp() const
{
	ASSERT(m_bNull   != true);
	ASSERT(m_oColumn.StgType() == MDST_TIMESTAMP);

	return *m_pTimeStamp;
}

CValue CField::GetValue() const
{
	// Check for null first.
	if (m_bNull)
		return null;

	// Decode type.
	switch(m_oColumn.StgType())
	{
		case MDST_INT:			return *m_pInt;
		case MDST_DOUBLE:		return *m_pDouble;
		case MDST_CHAR:			return *m_pChar;
		case MDST_STRING:		return m_pString;
		case MDST_BOOL:			return *m_pBool;
		case MDST_TIME_T:		return *m_pTimeT;
		case MDST_TIMESTAMP:	return (time_t) *m_pTimeStamp;
		case MDST_POINTER:		return m_pVoidPtr;
	}

	ASSERT_FALSE();

	return null;
}

void* CField::GetPtr() const
{
	ASSERT(m_bNull   != true);
	ASSERT(m_oColumn.ColType() == MDCT_VOIDPTR);

	return m_pVoidPtr;
}

CRow* CField::GetRowPtr() const
{
	ASSERT(m_bNull   != true);
	ASSERT(m_oColumn.ColType() == MDCT_ROWPTR);

	return m_pRowPtr;
}

CRow** CField::GetRowSetPtr() const
{
	ASSERT(m_bNull   != true);
	ASSERT(m_oColumn.ColType() == MDCT_ROWSETPTR);

	return m_pRowSetPtr;
}

/******************************************************************************
** Method:		GetRaw()
**
** Description:	Gets the value given into a raw buffer.
**
** Parameters:	pValue	The value to copy the field into.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CField::GetRaw(void* pValue) const
{
	// Variable buffer string?
	if (m_oColumn.ColType() == MDCT_VARSTR)
	{
		char* pszValue = (char*) pValue;

		strcpy(pszValue, m_pString);
	}
	// POINTER based type?
	else if (m_oColumn.StgType() == MDST_POINTER)
	{
		ASSERT(m_oColumn.AllocSize() == 0);

		memcpy(pValue, &m_pVoidPtr, sizeof(void*));

		// Is this right?
		ASSERT_FALSE();
	}
	else
	{
		ASSERT(m_oColumn.AllocSize() > 0);

		memcpy(pValue, m_pVoidPtr, m_oColumn.AllocSize());
	}
}

/******************************************************************************
** Method:		Set"Type"()
**
** Description:	Assigns a new value.
**
** Parameters:	Value	The new value.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CField::SetNull()
{
	ASSERT(m_oColumn.Nullable());
	ASSERT(!(m_oRow.InTable() && m_oColumn.ReadOnly()));
	ASSERT(!(m_oRow.InTable() && (m_oColumn.Index() != NULL)));

	if (m_bNull == true)
		return;

	m_bNull = true;

	Updated();
}

void CField::SetInt(int iValue)
{
	ASSERT(m_oColumn.StgType() == MDST_INT);
	ASSERT(!(m_oRow.InTable() && m_oColumn.ReadOnly()));
	ASSERT(!(m_oRow.InTable() && (m_oColumn.Index() != NULL)));

#ifdef _DEBUG
	CTable* pFKTable  = m_oColumn.FKTable();
	int     nFKColumn = m_oColumn.FKColumn();

	// If foreign key column, check value exists.
	if (pFKTable != NULL)
	{
		ASSERT(pFKTable->SelectRow(nFKColumn, iValue) != NULL);
	}
#endif //_DEBUG

	if ( (m_bNull == false) && (*m_pInt == iValue) )
		return;

#ifdef _DEBUG
	if (m_oRow.InTable())
		m_oRow.Table().CheckColumn(m_oRow, m_nColumn, iValue, true);
#endif //_DEBUG

	*m_pInt = iValue;
	m_bNull = false;

	Updated();
}

void CField::SetDouble(double dValue)
{
	ASSERT(m_oColumn.StgType() == MDST_DOUBLE);
	ASSERT(!(m_oRow.InTable() && m_oColumn.ReadOnly()));
	ASSERT(!(m_oRow.InTable() && (m_oColumn.Index() != NULL)));

	if ( (m_bNull == false) && (*m_pDouble == dValue) )
		return;

#ifdef _DEBUG
	if (m_oRow.InTable())
		m_oRow.Table().CheckColumn(m_oRow, m_nColumn, dValue, true);
#endif //_DEBUG

	*m_pDouble = dValue;
	m_bNull = false;

	Updated();
}

void CField::SetChar(char cValue)
{
	ASSERT(m_oColumn.StgType() == MDST_CHAR);
	ASSERT(!(m_oRow.InTable() && m_oColumn.ReadOnly()));
	ASSERT(!(m_oRow.InTable() && (m_oColumn.Index() != NULL)));

	if ( (m_bNull == false) && (*m_pChar == cValue) )
		return;

#ifdef _DEBUG
	if (m_oRow.InTable())
		m_oRow.Table().CheckColumn(m_oRow, m_nColumn, cValue, true);
#endif //_DEBUG

	*m_pChar = cValue;
	m_bNull = false;

	Updated();
}

void CField::SetString(const char* sValue)
{
	ASSERT(m_oColumn.StgType() == MDST_STRING);
	ASSERT(m_oColumn.Length()  >= (int)strlen(sValue));
	ASSERT(!(m_oRow.InTable() && m_oColumn.ReadOnly()));
	ASSERT(!(m_oRow.InTable() && (m_oColumn.Index() != NULL)));

	if ( (m_bNull == false) && (strcmp(m_pString, sValue) == 0) )
		return;

#ifdef _DEBUG
	if (m_oRow.InTable())
		m_oRow.Table().CheckColumn(m_oRow, m_nColumn, sValue, true);
#endif //_DEBUG

	// Variable buffer string?
	if (m_oColumn.ColType() == MDCT_VARSTR)
		m_pString = (char*) realloc(m_pString, strlen(sValue)+1);

	strcpy(m_pString, sValue);
	m_bNull = false;

	Updated();
}

void CField::SetBool(bool bValue)
{
	ASSERT(m_oColumn.StgType() == MDST_BOOL);
	ASSERT(!(m_oRow.InTable() && m_oColumn.ReadOnly()));
	ASSERT(!(m_oRow.InTable() && (m_oColumn.Index() != NULL)));

	if ( (m_bNull == false) && (*m_pBool == bValue) )
		return;

#ifdef _DEBUG
	if (m_oRow.InTable())
		m_oRow.Table().CheckColumn(m_oRow, m_nColumn, bValue, true);
#endif //_DEBUG

	*m_pBool = bValue;
	m_bNull = false;

	Updated();
}

void CField::SetDateTime(time_t tValue)
{
	ASSERT(m_oColumn.StgType() == MDST_TIME_T);
	ASSERT(!(m_oRow.InTable() && m_oColumn.ReadOnly()));
	ASSERT(!(m_oRow.InTable() && (m_oColumn.Index() != NULL)));

	if ( (m_bNull == false) && (*m_pTimeT == tValue) )
		return;

#ifdef _DEBUG
	if (m_oRow.InTable())
		m_oRow.Table().CheckColumn(m_oRow, m_nColumn, tValue, true);
#endif //_DEBUG

	*m_pTimeT = tValue;
	m_bNull = false;

	Updated();
}

void CField::SetTimeStamp(const CTimeStamp& tsValue)
{
	ASSERT(m_oColumn.StgType() == MDST_TIMESTAMP);
	ASSERT(!(m_oRow.InTable() && m_oColumn.ReadOnly()));
	ASSERT(!(m_oRow.InTable() && (m_oColumn.Index() != NULL)));

	if ( (m_bNull == false) && (*m_pTimeStamp == tsValue) )
		return;

#ifdef _DEBUG
	if (m_oRow.InTable())
		m_oRow.Table().CheckColumn(m_oRow, m_nColumn, (time_t)tsValue, true);
#endif //_DEBUG

	*m_pTimeStamp = tsValue;
	m_bNull = false;

	Updated();
}

void CField::SetField(const CField& oValue)
{
	ASSERT(m_oColumn.StgType() == oValue.m_oColumn.StgType());

	if (oValue.m_bNull)
	{
		SetNull();
	}
	else
	{
		switch(oValue.m_oColumn.StgType())
		{
			case MDST_INT:		SetInt     (*oValue.m_pInt);	break;
			case MDST_DOUBLE:	SetDouble  (*oValue.m_pDouble);	break;
			case MDST_CHAR:		SetChar    (*oValue.m_pChar);	break;
			case MDST_STRING:	SetString  (oValue.m_pString);	break;
			case MDST_BOOL:		SetBool    (*oValue.m_pBool);	break;
			case MDST_TIME_T:	SetDateTime(*oValue.m_pTimeT);	break;
			default:			ASSERT_FALSE();					break;
		}
	}
}

void CField::SetPtr(void* pValue)
{
	ASSERT(m_oColumn.StgType() == MDST_POINTER);
	ASSERT(!(m_oRow.InTable() && m_oColumn.ReadOnly()));
	ASSERT(pValue != NULL);

	if ( (m_bNull == false) && (m_pVoidPtr == pValue) )
		return;

	m_pVoidPtr = pValue;
	m_bNull = false;

	Updated();
}

void CField::SetRowPtr(CRow* pValue)
{
	ASSERT(m_oColumn.StgType() == MDST_POINTER);
	ASSERT(!(m_oRow.InTable() && m_oColumn.ReadOnly()));
	ASSERT(pValue != NULL);

	if ( (m_bNull == false) && (m_pRowPtr == pValue) )
		return;

	m_pRowPtr  = pValue;
	m_bNull = false;

	Updated();
}

void CField::SetRowSetPtr(CRow** pValue)
{
	ASSERT(m_oColumn.StgType() == MDST_POINTER);
	ASSERT(!(m_oRow.InTable() && m_oColumn.ReadOnly()));
	ASSERT(pValue != NULL);

	if ( (m_bNull == false) && (m_pRowSetPtr == pValue) )
		return;

	m_pRowSetPtr  = pValue;
	m_bNull = false;

	Updated();
}

/******************************************************************************
** Method:		SetRaw()
**
** Description:	Sets the value given a raw buffer to the data.
**
** Parameters:	pValue	The value to set the field to.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CField::SetRaw(const void* pValue)
{
	// Variable buffer string?
	if (m_oColumn.ColType() == MDCT_VARSTR)
	{
		const char* pszValue = (const char*) pValue;

		m_pString = (char*) realloc(m_pString, strlen(pszValue)+1);
		strcpy(m_pString, pszValue);
	}
	// POINTER based type?
	else if (m_oColumn.StgType() == MDST_POINTER)
	{
		ASSERT(m_oColumn.AllocSize() == 0);

		m_pVoidPtr = const_cast<void*>(pValue);

		// Is this right?
		ASSERT_FALSE();
	}
	else
	{
		ASSERT(m_oColumn.AllocSize() > 0);

		memcpy(m_pVoidPtr, pValue, m_oColumn.AllocSize());
	}

	m_bNull = false;

//	Updated();
}

/******************************************************************************
** Methods:		operator==()
**
** Description:	Compares the field to a value.
**
** Parameters:	oValue	The value to compare to.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CField::operator==(const CValue& oValue) const
{
	// Check NULLs first.
	if ( (m_bNull) && (oValue.m_bNull) )
		return true;

	if ( ((m_bNull) && (!oValue.m_bNull)) || ((!m_bNull) && (oValue.m_bNull)) )
		return false;

	ASSERT(m_oColumn.StgType() == oValue.m_eType);

	// Compare according to storage type.
	switch(oValue.m_eType)
	{
		case MDST_INT:		return (*m_pInt     == oValue.m_iValue);
		case MDST_DOUBLE:	return (*m_pDouble  == oValue.m_dValue);
		case MDST_CHAR:		return (*m_pChar    == oValue.m_cValue);
		case MDST_STRING:	return (StrCmp(oValue.m_sValue) == 0);
		case MDST_BOOL:		return (*m_pBool    == oValue.m_bValue);
		case MDST_TIME_T:	return (*m_pTimeT   == oValue.m_tValue);
		case MDST_POINTER:	return (m_pVoidPtr  == oValue.m_pValue);
	}

	ASSERT_FALSE();

	return false;
}

////////////////////////////////////////////////////////////////////////////////
//! Helper function for comparing values larger than an int.

template<typename T>
int Compare(const T& lhs, const T& rhs)
{
	if (lhs < rhs)
		return -1;

	if (rhs < lhs)
		return +1;

	return 0;
}

/******************************************************************************
** Methods:		Compare()
**
** Description:	Compares the field to another field or value.
**
** Parameters:	oValue		The value to compare to.
**
** Returns:		< 0		If less than.
**				= 0		If equal.
**				> 0		If greater than.
**
*******************************************************************************
*/

int CField::Compare(const CField& oValue) const
{
	ASSERT(m_oColumn.StgType() == oValue.m_oColumn.StgType());

	// Handles nulls.
	if (m_bNull)
		return (oValue.m_bNull) ? 0 : -1;
	else if (oValue.m_bNull)
		return 1;

	int nCmp = 0;

	// Compare according to storage type.
	switch(m_oColumn.StgType())
	{
		case MDST_INT:			nCmp = (*m_pInt   - *oValue.m_pInt);			break;
		case MDST_DOUBLE:		nCmp = ::Compare(*m_pDouble, *oValue.m_pDouble);	break;
		case MDST_CHAR:			nCmp = (*m_pChar  - *oValue.m_pChar);			break;
		case MDST_STRING:		nCmp = StrCmp(oValue.m_pString);				break;
		case MDST_BOOL:			nCmp = (*m_pBool  - *oValue.m_pBool);			break;
		case MDST_TIME_T:		nCmp = ::Compare(*m_pTimeT, *oValue.m_pTimeT);	break;
		case MDST_TIMESTAMP:	ASSERT_FALSE();									break;
		case MDST_POINTER:		ASSERT_FALSE();									break;
		default:				ASSERT_FALSE();									break;
	}

	return nCmp;
}

int CField::Compare(const CValue& oValue) const
{
	ASSERT(m_oColumn.StgType() == oValue.m_eType);

	// Handles nulls.
	if (m_bNull)
		return (oValue.m_bNull) ? 0 : -1;
	else if (oValue.m_bNull)
		return 1;

	int nCmp = 0;

	// Compare according to storage type.
	switch(m_oColumn.StgType())
	{
		case MDST_INT:			nCmp = (*m_pInt   - oValue.m_iValue);			break;
		case MDST_DOUBLE:		nCmp = ::Compare(*m_pDouble, oValue.m_dValue);	break;
		case MDST_CHAR:			nCmp = (*m_pChar  - oValue.m_cValue);			break;
		case MDST_STRING:		nCmp = StrCmp(oValue.m_sValue);					break;
		case MDST_BOOL:			nCmp = (*m_pBool  - oValue.m_bValue);			break;
		case MDST_TIME_T:		nCmp = ::Compare(*m_pTimeT, oValue.m_tValue);	break;
		case MDST_TIMESTAMP:	ASSERT_FALSE();									break;
		case MDST_POINTER:		ASSERT_FALSE();									break;
		default:				ASSERT_FALSE();									break;
	}

	return nCmp;
}

/******************************************************************************
** Methods:		Updated()
**
** Description:	Sets the modified flag for the field and its parent row.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CField::Updated()
{
	// Row is part of table AND is not a transient column?
	if ( (m_oRow.InTable() == true) && (m_oColumn.Transient() == false))
	{
		m_bModified = true;
		m_oRow.MarkUpdated();
		m_oRow.Table().m_nUpdates++;
	}
}

/******************************************************************************
** Methods:		Format()
**
** Description:	Convert the value to a string.
**
** Parameters:	pszFormat	The format specifiers
**							NB: If NULL uses the default.
**
** Returns:		The value as a string.
**
*******************************************************************************
*/

CString CField::Format(const char* pszFormat) const
{
	// Null?
	if (m_bNull)
		return "";

	// Use default specifier, if not supplied.
	if (pszFormat == NULL)
		pszFormat = pszFormats[m_oColumn.ColType()];

	ASSERT(pszFormat != NULL);

	CString str;

	// Format according to column type.
	switch(m_oColumn.ColType())
	{
		case MDCT_INT:			str.Format(pszFormat, *m_pInt);		break;
		case MDCT_DOUBLE:		str.Format(pszFormat, *m_pDouble);	break;
		case MDCT_CHAR:			str.Format(pszFormat, *m_pChar);	break;
		case MDCT_FXDSTR:		str = m_pString;					break;
		case MDCT_VARSTR:		str = m_pString;					break;
		case MDCT_BOOL:			str = FormatBool(pszFormat);		break;                                  
		case MDCT_IDENTITY:		str.Format(pszFormat, *m_pInt);		break;
		case MDCT_DATETIME:		str = FormatTimeT(pszFormat);		break;
		case MDCT_DATE:			str = FormatTimeT(pszFormat);		break;
		case MDCT_TIME:			str = FormatTimeT(pszFormat);		break;
		case MDCT_TIMESTAMP:	str = FormatTimeStamp(pszFormat);	break;
		case MDCT_VOIDPTR:		str.Format(pszFormat, m_pVoidPtr);	break;
		case MDCT_ROWPTR:		str.Format(pszFormat, m_pVoidPtr);	break;
		case MDCT_ROWSETPTR:	str.Format(pszFormat, m_pVoidPtr);	break;
		default:				ASSERT_FALSE();						break;
	}

	return str;
}

/******************************************************************************
** Methods:		DbgFormat()
**
** Description:	Convert the value to a debug string.
**
** Parameters:	None.
**
** Returns:		The value as a string.
**
*******************************************************************************
*/

CString CField::DbgFormat() const
{
	// Null?
	if (m_bNull)
		return "(null)";

	CString str;

	// Format according to storage type.
	switch(m_oColumn.StgType())
	{
		case MDST_INT:			str.Format("%d", *m_pInt);					break;
		case MDST_DOUBLE:		str.Format("%f", *m_pDouble);				break;
		case MDST_CHAR:			str.Format("%c", *m_pChar);					break;
		case MDST_STRING:		str = m_pString;							break;
		case MDST_BOOL:			str = FormatBool("N|Y");					break;
		case MDST_TIME_T:		str = FormatTimeT("%d/%m/%y %H:%M:%S");		break;
		case MDST_TIMESTAMP:	str = FormatTimeStamp("%d/%m/%y %H:%M:%S");	break;
		case MDST_POINTER:		str.Format("%p", m_pVoidPtr);				break;
		default:				ASSERT_FALSE();								break;
	}

	// Replace any CRs or LFs with a '.'.
	for (char* psz = const_cast<char*>((const char*)str); *psz != '\0'; psz++)
	{
		if (iscntrl(*psz))
			*psz = '.';
	}

	return str;
}

/******************************************************************************
** Methods:		FormatTimeT()
**
** Description:	Convert the time_t value to a string.
**
** Parameters:	pszFormat	The format string.
**
** Returns:		The value as a string.
**
*******************************************************************************
*/

CString CField::FormatTimeT(const char* pszFormat) const
{
	ASSERT(pszFormat != NULL);

	char szTime[100];
	tm*  pTM = NULL;

	// Convert the tm struct.
	if (m_oColumn.Flags() & CColumn::TZ_GMT)
		pTM = gmtime(m_pTimeT);
	else
		pTM = localtime(m_pTimeT);

	// Format.
	strftime(szTime, sizeof(szTime), pszFormat, pTM);

	return szTime;
}

/******************************************************************************
** Methods:		FormatTimeT()
**
** Description:	Convert the TimeStamp value to a string.
**
** Parameters:	pszFormat	The format string as for time_t types.
**
** Returns:		The value as a string.
**
*******************************************************************************
*/

CString CField::FormatTimeStamp(const char* pszFormat) const
{
	ASSERT(pszFormat != NULL);

	char szTime[100];
	tm	 oTM;

	// Initialise the struct.
	memset(&oTM, 0, sizeof(oTM));

	oTM.tm_mday  = m_pTimeStamp->day;
	oTM.tm_mon   = m_pTimeStamp->month-1;
	oTM.tm_year  = m_pTimeStamp->year;
	oTM.tm_hour  = m_pTimeStamp->hour;
	oTM.tm_min   = m_pTimeStamp->minute;
	oTM.tm_sec   = m_pTimeStamp->second;
	oTM.tm_isdst = (m_oColumn.Flags() & CColumn::TZ_GMT) ? 0 : 1;

	// Format.
	strftime(szTime, sizeof(szTime), pszFormat, &oTM);

	return szTime;
}

/******************************************************************************
** Methods:		FormatBool()
**
** Description:	Convert the bool value to a string.
**
** Parameters:	pszFormat	The format string which contains two strings
**							separated by a | character.
**
** Returns:		The value as a string.
**
*******************************************************************************
*/

CString CField::FormatBool(const char* pszFormat) const
{
	ASSERT(pszFormat != NULL);
	ASSERT(strchr(pszFormat, '|') != NULL);

	// Copy the whole string and find
	// the length and pos of the separator.
	CString str  = pszFormat;
	int     nLen = str.Length();
	int     nSep = str.Find('|');

	// Delete either the 1st or 2nd substring.
	if (*m_pBool == false)
		str.Delete(nSep, nLen-nSep);
	else
		str.Delete(0, nSep + 1);

	return str;
}

/******************************************************************************
** Methods:		StrCmp()
**
** Description:	Compares the string to another, using either a case sensitive
**				or case insensitive compare.
**
** Parameters:	pszRHS	The string to compare to.
**
** Returns:		As str(i)cmp.
**
*******************************************************************************
*/

int CField::StrCmp(const char* pszRHS) const
{
	bool bCmpCase = (m_oColumn.Flags() & CColumn::COMPARE_CASE);

	return (bCmpCase) ? strcmp(m_pString, pszRHS) : _stricmp(m_pString, pszRHS);
}
