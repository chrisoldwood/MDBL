/******************************************************************************
**
** MODULE:		ROW.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CRow class definition.
**
*******************************************************************************
*/

#include "MDBL.hpp"

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
	int i;
	int	nBufSize = 0;

	// Calculate the size of the data values buffer.
	nBufSize += m_nColumns * sizeof(CField);
	nBufSize += m_oTable.m_vColumns.AllocSize();

	// Allocate the buffer.
	m_aFields = (CField*) calloc(1, nBufSize);

	// Calculate start of data region.
	byte* pData = (byte*)(m_aFields + m_nColumns);

	// Initialise each field.
	for (i = 0; i < m_nColumns; i++)
	{
		CColumn& oColumn = m_oTable.m_vColumns[i];

		// Construct field using 'placement new'.
		new(&m_aFields[i]) CField(*this, oColumn, i, bNull, pData);

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
	for (int i = 0; i < m_nColumns; i++)
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

void CRow::Read(CStream& rStream)
{
	// Get the row data size and start address.
	int   nSize = m_oTable.m_vColumns.AllocSize();
	byte* pData = (byte*)(m_aFields + m_nColumns);

	// Read the null values.
	for (int i=0; i < m_nColumns; i++)
		rStream.Read(&m_aFields[i].m_bNull, sizeof(bool));

	// Read the data values..
	rStream.Read(pData, nSize);

	// Read any MDCT_VARSTR field values.
	for (i = 0; i < m_nColumns; i++)
	{
		if (m_aFields[i].m_oColumn.ColType() == MDCT_VARSTR)
		{
			int nLen;

			// Read the string length.
			rStream.Read(&nLen, sizeof(int));

			// Allocate the buffer.
			m_aFields[i].m_pString = (char*) realloc(m_aFields[i].m_pString, nLen+1);

			// Read the string.
			rStream.Read(m_aFields[i].m_pString, nLen+1);
		}
	}

	// Set status flag.
	m_eStatus = ORIGINAL;
}

void CRow::Write(CStream& rStream)
{
	// Get the row data size and start address.
	int   nSize = m_oTable.m_vColumns.AllocSize();
	byte* pData = (byte*)(m_aFields + m_nColumns);

	// Write the null values.
	for (int i=0; i < m_nColumns; i++)
		rStream.Write(&m_aFields[i].m_bNull, sizeof(bool));

	// Write the data values.
	rStream.Write(pData, nSize);

	// Write any MDCT_VARSTR field values.
	for (i = 0; i < m_nColumns; i++)
	{
		if (m_aFields[i].m_oColumn.ColType() == MDCT_VARSTR)
		{
			int nLen = strlen(m_aFields[i].m_pString);

			// Read the string length.
			rStream.Write(&nLen, sizeof(int));

			// Write the string.
			rStream.Write(m_aFields[i].m_pString, nLen+1);
		}
	}

	// Reset status flag.
	m_eStatus = ORIGINAL;
}
