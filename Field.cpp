/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FIELD.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CField class definition.
**
*******************************************************************************
*/

#include "MDBL.hpp"

// The special null value.
CNull null;

/******************************************************************************
** Method:		Constructor.
**
** Description:	WARNING: This is never actually called because CRow mallocs a
**				buffer and casts the return. Should really use placement new
**				syntax for this.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CField::CField()
{
	ASSERT(false);
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
	ASSERT(m_pColumn != NULL);
	ASSERT(m_pRow    != NULL);
	ASSERT(m_pColumn->StgType() == MDST_INT);

	return *m_pInt;
}

double CField::GetDouble() const
{
	ASSERT(m_bNull   != true);
	ASSERT(m_pColumn != NULL);
	ASSERT(m_pRow    != NULL);
	ASSERT(m_pColumn->StgType() == MDST_DOUBLE);

	return *m_pDouble;
}

char CField::GetChar() const
{
	ASSERT(m_bNull   != true);
	ASSERT(m_pColumn != NULL);
	ASSERT(m_pRow    != NULL);
	ASSERT(m_pColumn->StgType() == MDST_CHAR);

	return *m_pChar;
}

const char* CField::GetString() const
{
	ASSERT(m_bNull   != true);
	ASSERT(m_pColumn != NULL);
	ASSERT(m_pRow    != NULL);
	ASSERT(m_pColumn->StgType() == MDST_STRING);

	return m_pString;
}

bool CField::GetBool() const
{
	ASSERT(m_bNull   != true);
	ASSERT(m_pColumn != NULL);
	ASSERT(m_pRow    != NULL);
	ASSERT(m_pColumn->StgType() == MDST_BOOL);

	return *m_pBool;
}

time_t CField::GetDateTime() const
{
	ASSERT(m_bNull   != true);
	ASSERT(m_pColumn != NULL);
	ASSERT(m_pRow    != NULL);
	ASSERT(m_pColumn->StgType() == MDST_TIME_T);

	return *m_pTimeT;
}

const CTimeStamp& CField::GetTimeStamp() const
{
	ASSERT(m_bNull   != true);
	ASSERT(m_pColumn != NULL);
	ASSERT(m_pRow    != NULL);
	ASSERT(m_pColumn->ColType() == MDCT_TIMESTAMP);

	return *m_pTimeStamp;
}

CValue CField::GetValue() const
{
	ASSERT(m_pColumn != NULL);
	ASSERT(m_pRow    != NULL);

	// Check for null first.
	if (m_bNull)
		return null;

	// Decode type.
	switch(m_pColumn->StgType())
	{
		case MDST_INT:		return *m_pInt;
		case MDST_DOUBLE:	return *m_pDouble;
		case MDST_CHAR:		return *m_pChar;
		case MDST_STRING:	return m_pString;
		case MDST_BOOL:		return *m_pBool;
		case MDST_TIME_T:	return *m_pTimeT;
	}

	ASSERT(false);

	return null;
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
	ASSERT(m_pColumn != NULL);
	ASSERT(m_pRow    != NULL);
	ASSERT(m_pColumn->Nullable());

	if (m_bNull == true)
		return;

	m_bNull = true;

	m_pRow->AddStatus(CRow::UPDATED);
}

void CField::SetInt(int iValue)
{
	ASSERT(m_pColumn != NULL);
	ASSERT(m_pRow    != NULL);
	ASSERT(m_pColumn->StgType() == MDST_INT);

#ifdef _DEBUG
	CTable* pFKTable  = m_pColumn->FKTable();
	int     nFKColumn = m_pColumn->FKColumn();

	// If foreign key column, check value exists.
	if (pFKTable != NULL)
	{
		ASSERT(pFKTable->SelectRow(nFKColumn, iValue) != NULL);
	}
#endif //_DEBUG

	if ( (m_bNull == false) && (*m_pInt == iValue) )
		return;

	*m_pInt = iValue;
	m_bNull = false;

	m_pRow->AddStatus(CRow::UPDATED);
}

void CField::SetDouble(double dValue)
{
	ASSERT(m_pColumn != NULL);
	ASSERT(m_pRow    != NULL);
	ASSERT(m_pColumn->StgType() == MDST_DOUBLE);

	if ( (m_bNull == false) && (*m_pDouble == dValue) )
		return;

	*m_pDouble = dValue;
	m_bNull = false;

	m_pRow->AddStatus(CRow::UPDATED);
}

void CField::SetChar(char cValue)
{
	ASSERT(m_pColumn != NULL);
	ASSERT(m_pRow    != NULL);
	ASSERT(m_pColumn->StgType() == MDST_CHAR);

	if ( (m_bNull == false) && (*m_pChar == cValue) )
		return;

	*m_pChar = cValue;
	m_bNull = false;

	m_pRow->AddStatus(CRow::UPDATED);
}

void CField::SetString(const char* sValue)
{
	ASSERT(m_pColumn != NULL);
	ASSERT(m_pRow    != NULL);
	ASSERT(m_pColumn->StgType() == MDST_STRING);
	ASSERT(m_pColumn->Length()  >= (int)strlen(sValue));

	if ( (m_bNull == false) && (strcmp(m_pString, sValue) == 0) )
		return;

	strcpy(m_pString, sValue);
	m_pString[m_pColumn->Length()] = '\0';
	m_bNull = false;

	m_pRow->AddStatus(CRow::UPDATED);
}

void CField::SetBool(bool bValue)
{
	ASSERT(m_pColumn != NULL);
	ASSERT(m_pRow    != NULL);
	ASSERT(m_pColumn->StgType() == MDST_BOOL);

	if ( (m_bNull == false) && (*m_pBool == bValue) )
		return;

	*m_pBool = bValue;
	m_bNull = false;

	m_pRow->AddStatus(CRow::UPDATED);
}

void CField::SetDateTime(time_t tValue)
{
	ASSERT(m_pColumn != NULL);
	ASSERT(m_pRow    != NULL);
	ASSERT(m_pColumn->StgType() == MDST_TIME_T);

	if ( (m_bNull == false) && (*m_pTimeT == tValue) )
		return;

	*m_pTimeT = tValue;
	m_bNull = false;

	m_pRow->AddStatus(CRow::UPDATED);
}

void CField::SetTimeStamp(const CTimeStamp& tsValue)
{
	ASSERT(m_pColumn != NULL);
	ASSERT(m_pRow    != NULL);
	ASSERT(m_pColumn->ColType() == MDCT_TIMESTAMP);

	if ( (m_bNull == false) && (*m_pTimeStamp == tsValue) )
		return;

	*m_pTimeStamp = tsValue;
	m_bNull = false;

	m_pRow->AddStatus(CRow::UPDATED);
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
	ASSERT(m_pColumn != NULL);
	ASSERT(m_pRow    != NULL);

	memcpy(m_pVoid, pValue, m_pColumn->AllocSize());
	m_bNull = false;

	m_pRow->AddStatus(CRow::UPDATED);
}

/******************************************************************************
** Method:		operator==()
**
** Description:	Compares the field to a CValue object.
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

	ASSERT(m_pColumn != NULL);
	ASSERT(m_pColumn->StgType() == oValue.m_eType);

	// Compare according to storage type.
	switch(oValue.m_eType)
	{
		case MDST_INT:		return (*m_pInt    == oValue.m_iValue);
		case MDST_DOUBLE:	return (*m_pDouble == oValue.m_dValue);
		case MDST_CHAR:		return (*m_pChar   == oValue.m_cValue);
		case MDST_STRING:	return (strcmp(m_pString, oValue.m_sValue) == 0);
		case MDST_BOOL:		return (*m_pBool   == oValue.m_bValue);
		case MDST_TIME_T:	return (*m_pTimeT  == oValue.m_tValue);
	}

	ASSERT(false);

	return false;
}
