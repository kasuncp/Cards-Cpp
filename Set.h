#ifndef _KITZ_SET_HEADER_
#define _KITZ_SET_HEADER_

#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <vector>
#include <list>

using namespace std;

template <typename T>
class Set
{
public:
	Set();
	virtual ~Set();

	virtual void Sort();
	virtual void Shuffle(int iNumOfTurns);

	void	PushFront(T* pItem);
	void	PopFront();
	void	PushBack(T* pItem);
	void	PopBack();
	
	int		Size();
	void	Clear();
	void	CopyTo(Set<T>*& pCopySet);

	virtual void DebugPrint();

	//Iterator interface
	void	Begin();
	T*		GetNext();
	bool	HasNext();

protected:
	list<T*>	lst_Items;
	typename list<T*>::iterator iter_Next;

private:
	int		Partition(vector<T*>& vecInput, int p, int r);
	void	QuickSort(vector<T*>& vecInput, int p, int r);
	
};


#include "Set.inl"


#endif // _KITZ_SET_HEADER_