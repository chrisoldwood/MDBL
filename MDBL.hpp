/******************************************************************************
**
** MODULE:		MDBL.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	Wrapper to include all the core library headers.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef MDBL_HPP
#define MDBL_HPP

/******************************************************************************
**
** Standard headers.
**
*******************************************************************************
*/

#include "wcl.hpp"
#include <time.h>

/******************************************************************************
**
** Library specific headers.
**
*******************************************************************************
*/

#include "MDBLTypes.hpp"
#include "TimeStamp.hpp"

#include "Value.hpp"
#include "Field.hpp"

#include "Column.hpp"
#include "ColumnSet.hpp"

#include "Row.hpp"
#include "RowSet.hpp"
#include "ResultSet.hpp"

#include "Index.hpp"
#include "UniqIndex.hpp"
#include "IntMapIndex.hpp"
#include "StrMapIndex.hpp"

#include "Where.hpp"
#include "Compare.hpp"
#include "WhereExp.hpp"

#include "JoinedSet.hpp"
#include "Join.hpp"

#include "SQLException.hpp"
#include "SQLCursor.hpp"
#include "SQLSource.hpp"

#include "ODBCException.hpp"
#include "ODBCCursor.hpp"
#include "ODBCSource.hpp"

#include "Table.hpp"
#include "TableSet.hpp"

#include "MDB.hpp"


#endif //MDBL_HPP
