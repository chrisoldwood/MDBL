/******************************************************************************
**
** MODULE:		TIMESTAMP.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CTimeStamp class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "TimeStamp.hpp"
#include <time.h>

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

CTimeStamp::CTimeStamp()
{
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

CTimeStamp::~CTimeStamp()
{
}

/******************************************************************************
** Method:		ToString()
**
** Description:	Convert the timestamp to a string.
**
** Parameters:	bDate	Include the date?
**				bTime	Include the time?
**
** Returns:		The string.
**
*******************************************************************************
*/

CString CTimeStamp::ToString(bool bDate, bool bTime) const
{
	CString strDate, strTime;

	// Create strings.
	if (bDate)
		strDate.Format(TXT("%02d/%02d/%04d"), static_cast<int>(day), static_cast<int>(month), static_cast<int>(year));

	if (bTime)
		strTime.Format(TXT("%02d:%02d:%02d"), static_cast<int>(hour), static_cast<int>(minute), static_cast<int>(second));

	// Return one half only?
	if (!bTime)		return strDate;
	if (!bDate)		return strTime;

	CString strDateTime;

	// Create full string.
	strDateTime  = strDate;
	strDateTime += TXT(" ");
	strDateTime += strTime;

	return strDateTime;
}

/******************************************************************************
** Method:		operator time_t()
**
** Description:	Convert the timestamp to a time_t.
**
** Parameters:	None.
**
** Returns:		A time_t.
**
*******************************************************************************
*/

CTimeStamp::operator time_t() const
{
	struct tm oTime;

	oTime.tm_mday = day;
	oTime.tm_mon  = month - 1;
	oTime.tm_year = year - 1900;
	oTime.tm_hour = hour;
	oTime.tm_min  = minute;
	oTime.tm_sec  = second;

	oTime.tm_wday  = 0;
	oTime.tm_yday  = 0;
	oTime.tm_isdst = -1;

	return mktime(&oTime);
}

/******************************************************************************
** Method:		operator=()
**
** Description:	Convert the time_t to a timestamp.
**
** Parameters:	tTime	The date & time as a time_t.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CTimeStamp& CTimeStamp::operator=(time_t tTime)
{
	struct tm* pTime = localtime(&tTime);

	year     = static_cast<SQLSMALLINT> (pTime->tm_year + 1900);
	month    = static_cast<SQLUSMALLINT>(pTime->tm_mon  + 1);
	day      = static_cast<SQLUSMALLINT>(pTime->tm_mday);
	hour     = static_cast<SQLUSMALLINT>(pTime->tm_hour);
	minute   = static_cast<SQLUSMALLINT>(pTime->tm_min);
	second   = static_cast<SQLUSMALLINT>(pTime->tm_sec);
	fraction = 0;

	return *this;
}
