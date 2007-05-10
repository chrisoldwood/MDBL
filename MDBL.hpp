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

#include <Core/Common.hpp>		// Core library common headers.
#include <WCL/wcl.hpp>			// Windows C++ library.
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
#include "ValueSet.hpp"
#include "Field.hpp"

#include "Column.hpp"
#include "ColumnSet.hpp"

#include "Row.hpp"
#include "RowSet.hpp"

#include "Where.hpp"
#include "WhereCmp.hpp"
#include "WhereExp.hpp"
#include "WhereIn.hpp"
#include "WhereNot.hpp"

#include "SortColumns.hpp"
#include "ResultSet.hpp"
#include "GroupSet.hpp"

#include "Index.hpp"
#include "UniqIndex.hpp"
#include "IntMapIndex.hpp"
#include "StrMapIndex.hpp"

#include "JoinedSet.hpp"
#include "Join.hpp"

#include "SQLException.hpp"
#include "SQLCursor.hpp"
#include "SQLParams.hpp"
#include "SQLSource.hpp"
#include "AutoTrans.hpp"

#include "ODBCException.hpp"
#include "ODBCCursor.hpp"
#include "ODBCParams.hpp"
#include "ODBCSource.hpp"

#include "Table.hpp"
#include "TableSet.hpp"

#include "MDB.hpp"


#endif //MDBL_HPP
