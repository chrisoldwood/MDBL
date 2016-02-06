/******************************************************************************
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

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __GNUC__
#include <sqltypes.h>
#else
#include <sql.h>
#endif

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

	//! Construction from the discrete timestamp fields.
	CTimeStamp(SQLSMALLINT year_, SQLUSMALLINT month_, SQLUSMALLINT day_,
				SQLUSMALLINT hour_, SQLUSMALLINT minute_, SQLUSMALLINT second_,
				SQLUINTEGER fraction_ = 0);

	~CTimeStamp();

	//
	// Conversion methods.
	//

	//! Convert the timestamp to a time_t value.
	time_t ToTimeT() const;

	//! Initalise the timestamp from a time_t value.
	void FromTimeT(time_t time);

	CString ToString(bool bDate = true, bool bTime = true) const;

	//
	// Operators.
	//
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
	return ( (year     == oRHS.year    )
		  && (month    == oRHS.month   )
		  && (day      == oRHS.day     )
		  && (hour     == oRHS.hour    )
		  && (minute   == oRHS.minute  )
		  && (second   == oRHS.second  )
		  && (fraction == oRHS.fraction)
		   );
}

inline bool CTimeStamp::operator!=(const CTimeStamp& oRHS) const
{
	return !operator==(oRHS);
}

#endif //TIMESTAMP_HPP
