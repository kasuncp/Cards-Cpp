#ifndef _KITZ_CARD_FACTORY_HEADER_
#define _KITZ_CARD_FACTORY_HEADER_

#include "Set.h"
#include "Card.h"

class CardFactory
{
public:
	~CardFactory();

	static CardFactory* Instance();

	Set<Card>* GetCardsBySuit(int iSuit);
	Set<Card>* GetCardsByKit(int iKit);
	Set<Card>* GetAllCards();

protected:
	CardFactory();

private:
	static CardFactory* p_Instance;
};

CardFactory* CardFactory::p_Instance = NULL;

#endif // _KITZ_CARD_FACTORY_HEADER_