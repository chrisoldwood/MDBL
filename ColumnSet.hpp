/******************************************************************************
**
** MODULE:		COLUMNSET.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CColumnSet class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef COLUMNSET_HPP
#define COLUMNSET_HPP

/******************************************************************************
** 
** The class used to store the columns which belong to a table.
**
*******************************************************************************
*/

class CColumnSet : protected TPtrArray<CColumn>
{
public:
	//
	// Constructors/Destructor.
	//
	CColumnSet();
	~CColumnSet();
	
	//
	// Methods.
	//
	int      Count() const;
	CColumn& Column(int n) const;
	CColumn& operator[](int n) const;

	int  Add(CColumn& oColumn);
	void Delete(int n);
	void DeleteAll();
	int  Find(const char* pszName);

	int AllocSize() const;

protected:
	//
	// Members.
	//

private:
	//
	// Disallow copy and assignment.
	//
	CColumnSet(const CColumnSet&);
	void operator=(const CColumnSet&);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline int CColumnSet::Count() const
{
	return TPtrArray<CColumn>::Size();
}

inline CColumn& CColumnSet::Column(int n) const
{
	return *(TPtrArray<CColumn>::At(n));
}

inline CColumn& CColumnSet::operator[](int n) const
{
	return *(TPtrArray<CColumn>::At(n));
}

inline int CColumnSet::Add(CColumn& oColumn)
{
	return TPtrArray<CColumn>::Add(&oColumn);
}

#endif //COLUMNSET_HPP
