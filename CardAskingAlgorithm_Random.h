#ifndef _CARD_ASKING_ALGORITHM_RANDOM_HEADER_
#define _CARD_ASKING_ALGORITHM_RANDOM_HEADER_

#include "CardAskingAlgorithm.h"

class CardAskingAlgorithm_Random : public CardAskingAlgorithm
{
public:
	CardAskingAlgorithm_Random();
	~CardAskingAlgorithm_Random();

	Move* Execute();

protected:
private:
};

#endif // _CARD_ASKING_ALGORITHM_RANDOM_HEADER_