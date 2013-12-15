#ifndef _CARD_ASKING_ALGORITHM_HEADER_
#define _CARD_ASKING_ALGORITHM_HEADER_

#include "Player.h"

class CardAskingAlgorithm
{
public:
	CardAskingAlgorithm() {};
	virtual ~CardAskingAlgorithm() {};

	virtual Move* Execute() = 0;

protected:
private:
};

#endif // _CARD_ASKING_ALGORITHM_HEADER_