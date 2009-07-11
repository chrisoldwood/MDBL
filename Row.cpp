/******************************************************************************
**
** MODULE:		ROW.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CRow class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "Row.hpp"
#include "Table.hpp"
#include <WCL/IInputStream.hpp>
#include <WCL/IOutputStream.hpp>

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	oTable		The parent table.
**				bNull		Initialise ALL fields to NULL?
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CRow::CRow(CTable& oTable, bool bNull)
	: m_oTable(oTable)
	, m_aFields(NULL)
	, m_nColumns(oTable.m_vColumns.Count())
	, m_eStatus(ALLOCATED)
{
	size_t i;
	size_t nBufSize = 0;

	// Calculate the size of the data values buffer.
	nBufSize += m_nColumns * sizeof(CField);
	nBufSize += m_oTable.m_vColumns.AllocSize();

	// Allocate the buffer.
	m_aFields = static_cast<CField*>(calloc(1, nBufSize));

	// Calculate start of data region.
	byte* pData = reinterpret_cast<byte*>(m_aFields + m_nColumns);

	// Initialise each field.
	for (i = 0; i < m_nColumns; ++i)
	{
		CColumn& oColumn = m_oTable.m_vColumns[i];

#pragma push_macro("new")
#undef new 

		// Construct field using 'placement new'.
		new(&m_aFields[i]) CField(*this, oColumn, i, bNull, pData);

#pragma pop_macro("new")

		pData += oColumn.AllocSize();
	}
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

CRow::~CRow()
{
	// Destroy each field.
	for (size_t i = 0; i < m_nColumns; ++i)
		delete &m_aFields[i];

	if (m_aFields != NULL)
		free(m_aFields);
}

/******************************************************************************
** Methods:		Read()
**				Write()
**
** Description:	Operators to read/write the data from/to a stream.
**
** Parameters:	rStream		The stream.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CRow::Read(WCL::IInputStream& rStream)
{
	// Get the row data size and start address.
	size_t nSize = m_oTable.m_vColumns.AllocSize();
	byte* pData = reinterpret_cast<byte*>(m_aFields + m_nColumns);

	// Read the null values.
	for (size_t i=0; i < m_nColumns; ++i)
		rStream.Read(&m_aFields[i].m_bNull, sizeof(bool));

	// Read the data values..
	rStream.Read(pData, nSize);

	// Read any MDCT_VARSTR field values.
	for (size_t i = 0; i < m_nColumns; ++i)
	{
		if (m_aFields[i].m_oColumn.ColType() == MDCT_VARSTR)
		{
			size_t nChars;

			// Read the string length.
			rStream.Read(&nChars, sizeof(size_t));

			size_t nBytes = Core::numBytes<tchar>(nChars+1);

			// Allocate the buffer.
			m_aFields[i].m_pString = static_cast<tchar*>(realloc(m_aFields[i].m_pString, nBytes));

			// Read the string.
			rStream.Read(m_aFields[i].m_pString, nBytes);
		}
	}

	// Set status flag.
	m_eStatus = ORIGINAL;
}

void CRow::Write(WCL::IOutputStream& rStream)
{
	// Get the row data size and start address.
	size_t nSize = m_oTable.m_vColumns.AllocSize();
	byte* pData = reinterpret_cast<byte*>(m_aFields + m_nColumns);

	// Write the null values.
	for (size_t i=0; i < m_nColumns; ++i)
		rStream.Write(&m_aFields[i].m_bNull, sizeof(bool));

	// Write the data values.
	rStream.Write(pData, nSize);

	// Write any MDCT_VARSTR field values.
	for (size_t i = 0; i < m_nColumns; ++i)
	{
		if (m_aFields[i].m_oColumn.ColType() == MDCT_VARSTR)
		{
			size_t nChars = tstrlen(m_aFields[i].m_pString);
			size_t nBytes = Core::numBytes<tchar>(nChars+1);

			// Read the string length.
			rStream.Write(&nChars, sizeof(size_t));

			// Write the string.
			rStream.Write(m_aFields[i].m_pString, nBytes);
		}
	}

	// Reset status flag.
	m_eStatus = ORIGINAL;
}
