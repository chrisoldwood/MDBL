/******************************************************************************
**
** MODULE:		COMPARE.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CCompare class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef COMPARE_HPP
#define COMPARE_HPP

/******************************************************************************
** 
** This class is used for WHERE clause comparisons of values.
**
*******************************************************************************
*/

class CCompare : public CWhere
{
public:
	//
	// Methods
	//
	virtual bool Matches(const CRow& oRow) const = 0;

protected:
	//
	// Constructors/Destructor.
	//
	CCompare(int nColumn, const CValue& oValue);
	virtual ~CCompare();

	//
	// Members.
	//
	int		m_nColumn;	// The column to check.
	CValue	m_oValue;	// The value to match.
};

/******************************************************************************
** 
** These CCompare derived classes implement the actual comparisons.
**
*******************************************************************************
*/

class CWhereTrue : public CWhere
{
public:
	//
	// Methods
	//
	virtual bool Matches(const CRow& oRow) const;
};


class CWhereFalse : public CWhere
{
public:
	//
	// Methods
	//
	virtual bool Matches(const CRow& oRow) const;
};

class CWhereEqual : public CCompare
{
public:
	//
	// Constructors/Destructor.
	//
	CWhereEqual(int nColumn, const CValue& oValue);

	//
	// Methods
	//
	virtual bool Matches(const CRow& oRow) const;
};

class CWhereNotEqual : public CCompare
{
public:
	//
	// Constructors/Destructor.
	//
	CWhereNotEqual(int nColumn, const CValue& oValue);

	//
	// Methods
	//
	virtual bool Matches(const CRow& oRow) const;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CCompare::CCompare(int nColumn, const CValue& oValue)
	: m_nColumn(nColumn)
	, m_oValue(oValue)
{
}

inline CCompare::~CCompare()
{
}

inline bool CWhereTrue::Matches(const CRow&) const
{
	return true;
}

inline bool CWhereFalse::Matches(const CRow&) const
{
	return false;
}

inline CWhereEqual::CWhereEqual(int nColumn, const CValue& oValue)
	: CCompare(nColumn, oValue)
{
}

inline bool CWhereEqual::Matches(const CRow& oRow) const
{
	return (oRow[m_nColumn] == m_oValue);
}

inline CWhereNotEqual::CWhereNotEqual(int nColumn, const CValue& oValue)
	: CCompare(nColumn, oValue)
{
}

inline bool CWhereNotEqual::Matches(const CRow& oRow) const
{
	return (oRow[m_nColumn] != m_oValue);
}

#endif //COMPARE_HPP
