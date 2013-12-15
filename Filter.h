#ifndef _FILTER_HEADER_
#define _FILTER_HEADER_

#include "Set.h"
#include "Card.h"

class Filter
{
public:
	Filter();
	virtual ~Filter();

	virtual Set<Card>* Enter(Set<Card>* pInput) = 0;
};

#endif // _FILTER_HEADER_