/******************************************************************************
**
** MODULE:		AUTOTRANS.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CAutoTrans class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef AUTOTRANS_HPP
#define AUTOTRANS_HPP

/******************************************************************************
** 
** This is a helper class used to manage CSQLSource transactions.
**
*******************************************************************************
*/

class CAutoTrans
{
public:
	//
	// Constructors/Destructor.
	//
	CAutoTrans(CSQLSource& oSource);
	~CAutoTrans();

protected:
	//
	// Members.
	//
	CSQLSource& m_oSource;	// The SQLSource.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CAutoTrans::CAutoTrans(CSQLSource& oSource)
	: m_oSource(oSource)
{
	// Start a new transaction.
	m_oSource.BeginTrans();
}

inline CAutoTrans::~CAutoTrans()
{
	// If not commited, rollback.
	if (m_oSource.InTrans())
		m_oSource.RollbackTrans();
}

#endif //AUTOTRANS_HPP
