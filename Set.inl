template <typename T>
Set<T>::Set()
{
	srand(time(NULL));
}

template <typename T>
Set<T>::~Set()
{
	Clear();
}

template <typename T>
void Set<T>::Sort()
{
	vector<T*> vecTempItems(lst_Items.size());
	copy(lst_Items.begin(), lst_Items.end(), vecTempItems.begin());

	QuickSort(vecTempItems, 0, vecTempItems.size() - 1);

	list<T*> lstShuffled(vecTempItems.begin(), vecTempItems.end());
	lst_Items = lstShuffled;
}

template <typename T>
void Set<T>::Shuffle( int iNumOfTurns )
{
	vector<T*> vecTempItems(lst_Items.size());

	for( int iTurn = 0; iTurn < iNumOfTurns; ++iTurn )
	{
		int iLastPos = 0;
		for(list<T*>::iterator iter = lst_Items.begin(); iter != lst_Items.end(); iter++, iLastPos++) {
			int insertPos = rand() % (iLastPos + 1);
			if (insertPos < iLastPos) 
			{
				vecTempItems[iLastPos] = vecTempItems[insertPos];
			}

			vecTempItems[insertPos] = *iter;
		}

		list<T*> lstShuffled(vecTempItems.begin(), vecTempItems.end());
		lst_Items = lstShuffled;
	}
}

template <typename T>
void Set<T>::PushFront( T* pItem )
{
	if (pItem)
	{
		lst_Items.push_front(pItem);
	}
}

template <typename T>
void Set<T>::PopFront()
{
	lst_Items.pop_front();
}

template <typename T>
void Set<T>::PushBack( T* pItem )
{
	if (pItem)
	{
		lst_Items.push_back(pItem);
	}
}

template <typename T>
void Set<T>::PopBack()
{
	lst_Items.pop_back();
}

template <typename T>
void Set<T>::DebugPrint()
{
	for(list<T*>::iterator iter = lst_Items.begin(); iter != lst_Items.end(); ++iter)
	{
		(*iter)->DebugPrint();
	}
}

template <typename T>
int Set<T>::Partition(vector<T*>& vecInput, int p, int r)
{
	int pivot = vecInput[r]->GetID();

	while ( p < r )
	{
		while ( vecInput[p]->GetID() < pivot )
			p++;

		while ( vecInput[r]->GetID() > pivot )
			r--;

		if ( vecInput[p]->GetID() == vecInput[r]->GetID() )
			p++;
		
		else if ( p < r )
		{
			T* tmp = vecInput[p];
			vecInput[p] = vecInput[r];
			vecInput[r] = tmp;
		}
	}

	return r;
}

template <typename T>
void Set<T>::QuickSort(vector<T*>& vecInput, int p, int r)
{
	if ( p < r )
	{
		int j = Partition(vecInput, p, r);        
		QuickSort(vecInput, p, j-1);
		QuickSort(vecInput, j+1, r);
	}
}

template <typename T>
int Set<T>::Size()
{
	return lst_Items.size();
}


template <typename T>
void Set<T>::Clear()
{
// 	list<T*>::iterator iter = lst_Items.begin();
// 	for (; iter != lst_Items.end(); ++iter)
// 	{
// 		if ( (*iter) )
// 		{
// 			delete (*iter);
// 		}
// 	}

	lst_Items.clear();
}

template <typename T>
void Set<T>::CopyTo( Set<T>*& pCopySet )
{
	if ( !pCopySet )
	{
		pCopySet = new Set<T>();
	}

	list<T*>::iterator iter = lst_Items.begin();
	for (; iter != lst_Items.end(); ++iter)
	{
		pCopySet->PushBack((*iter));
	}
}

template <typename T>
void Set<T>::Begin()
{
	iter_Next = lst_Items.begin();
}

template <typename T>
T* Set<T>::GetNext()
{
	if (iter_Next != lst_Items.end())
	{
		list<T*>::iterator iter = iter_Next;
		++iter_Next;
		return (*iter);
	}

	return NULL;
}


template <typename T>
bool Set<T>::HasNext()
{
	return ( iter_Next != lst_Items.end() );
}
