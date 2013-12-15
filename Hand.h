#ifndef _HAND_HEADER_
#define _HAND_HEADER_

#include "Set.h"
#include "Card.h"

class Hand : public Set<Card>
{
public:
	Hand();
	virtual ~Hand();

	bool HasKit	( int iKitID  );
	bool HasCard( int iCardID );

	void	AddCard(Card* pCard);
	Card*	RemoveCard( int iCardID );

	void DebugPrint();

protected:
private:
};

#endif // _HAND_HEADER_