/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TIMESTAMP.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CTimeStamp class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef TIMESTAMP_HPP
#define TIMESTAMP_HPP

#include <sqltypes.h>

/******************************************************************************
** 
** This is a wrapper class for the TIMESTAMP_STRUCT type.
**
*******************************************************************************
*/

class CTimeStamp : public TIMESTAMP_STRUCT
{
public:
	//
	// Constructors/Destructor.
	//
	CTimeStamp();
	~CTimeStamp();
	
	//
	// Conversion methods.
	//
	CString ToString(bool bDate = true, bool bTime = true) const;

	//
	// Operators.
	//
	     operator time_t() const;
	void operator=(time_t tTime);
	bool operator==(const CTimeStamp& oRHS) const;
	bool operator!=(const CTimeStamp& oRHS) const;

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

inline bool CTimeStamp::operator==(const CTimeStamp& oRHS) const
{
	return false;
}

inline bool CTimeStamp::operator!=(const CTimeStamp& oRHS) const
{
	return !operator==(oRHS);
}

#endif //TIMESTAMP_HPP
