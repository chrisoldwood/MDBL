/******************************************************************************
** (C) Chris Oldwood
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
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CRow::CRow(CTable& oTable)
	: m_oTable(oTable)
	, m_aFields(NULL)
	, m_nColumns(oTable.m_vColumns.Size())
	, m_eStatus(ALLOCATED)
{
	int i;
	int	nBufSize = 0, nOffset = 0;

	// Calculate the size of the data values buffer.
	nBufSize += m_nColumns * sizeof(CField);
	nBufSize += m_oTable.m_vColumns.AllocSize();

	// Allocate the buffer.
	m_aFields = (CField*) calloc(1, nBufSize);

	// Initialise each column.
	for (i = 0; i < m_nColumns; i++)
	{
		CColumn& oColumn = m_oTable.m_vColumns[i];

		m_aFields[i].m_bNull   = false;
		m_aFields[i].m_pVoid   = ((byte*)(m_aFields + m_nColumns)) + nOffset;
		m_aFields[i].m_pColumn = &oColumn;
		m_aFields[i].m_pRow    = this;
		
		nOffset += oColumn.AllocSize();
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
	if (m_aFields != NULL)
		free(m_aFields);
}

/******************************************************************************
** Methods:		operator <<()
**				operator >>()
**
** Description:	Operators to read/write the data from/to a stream.
**
** Parameters:	rStream		The stream.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CRow::operator <<(CStream& rStream)
{
	// Get the row data size and start address.
	int   nSize = m_oTable.m_vColumns.AllocSize();
	byte* pData = (byte*)(m_aFields + m_nColumns);

	// Read the null values.
	for (int i=0; i < m_nColumns; i++)
		rStream >> m_aFields[i].m_bNull;

	// Read the data values..
	rStream.Read(pData, nSize);

	// Set status flag.
	m_eStatus = ORIGINAL;
}

void CRow::operator >>(CStream& rStream)
{
	// Get the row data size and start address.
	int   nSize = m_oTable.m_vColumns.AllocSize();
	byte* pData = (byte*)(m_aFields + m_nColumns);

	// Write the null values.
	for (int i=0; i < m_nColumns; i++)
		rStream << m_aFields[i].m_bNull;

	// Write the data values.
	rStream.Write(pData, nSize);

	// Reset status flag.
	m_eStatus = ORIGINAL;
}
