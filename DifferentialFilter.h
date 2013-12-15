#ifndef _DIFFERENTIAL_FILTER_HEADER_
#define _DIFFERENTIAL_FILTER_HEADER_

#include "Filter.h"

class DifferentialFilter : public Filter
{
public:
	DifferentialFilter();
	~DifferentialFilter();

	virtual Set<Card>* Enter(Set<Card>* pInput);

protected:
private:
};

#endif //_DIFFERENTIAL_FILTER_HEADER_