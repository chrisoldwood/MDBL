/******************************************************************************
**
** MODULE:		MDBLTYPES.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	Common data types, defines and macros.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef MDBLTYPES_HPP
#define MDBLTYPES_HPP

/******************************************************************************
**
** Primitive type aliases.
**
*******************************************************************************
*/

typedef unsigned char	byte;

/******************************************************************************
**
** Storage data types.
**
*******************************************************************************
*/

enum STGTYPE
{
	MDST_NULL,		// No type.
	MDST_INT,		// 32-bit int.
	MDST_DOUBLE,	// 64-bit floating-point.
	MDST_CHAR,		// Single character.
	MDST_STRING,	// String of characters.
	MDST_BOOL,      // True or False.
	MDST_TIME_T,    // time_t.
	MDST_TIMESTAMP, // CTimeStamp.
	MDST_POINTER,	// Generic pointer.
};

/******************************************************************************
**
** Column data types.
**
*******************************************************************************
*/

enum COLTYPE
{
	MDCT_INT,		// MDST_INT.
	MDCT_DOUBLE,	// MDST_DOUBLE.
	MDCT_CHAR,		// MDST_CHAR.
	MDCT_FXDSTR,	// MDST_STRING (using fixed length buffer).
	MDCT_VARSTR,	// MDST_STRING (using variable length buffer).
	MDCT_BOOL,		// MDST_BOOL
	MDCT_IDENTITY,	// MDST_INT (Identity column).
	MDCT_DATETIME,	// MDST_TIME_T.
	MDCT_TIMESTAMP, // CTimeStamp.
	MDCT_VOIDPTR,	// MDST_POINTER (using void*).
	MDCT_ROWPTR,	// MDST_POINTER (using CRow*).
	MDCT_ROWSETPTR,	// MDST_POINTER (using CRow*[]).
};

/******************************************************************************
**
** Special data type to represent a NULL value.
**
*******************************************************************************
*/

class CNull
{
};

extern CNull null;

#endif //MDBLTYPES_HPP
